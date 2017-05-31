#include "Common.hpp"
#include "MeshBehavior.hpp"
#include "Renderer/Renderer.hpp"
#include "App/App.hpp"
#include "Resource/ResourceSystem.hpp"
#include "Utils/GLUtils.hpp"
#include "Actor/Actor.hpp"

namespace
{
void reportGLError()
{
    int error = glGetError();
    switch (error)
    {
    case GL_NO_ERROR:
        return;
    case GL_INVALID_OPERATION:
        LOG(ERROR) << "GL ERROR: INVALID_OPERATION";
        return;
    case GL_INVALID_VALUE:
        LOG(ERROR) << "GL ERROR: INVALID_VALUE";
        return;
    default:
        LOG(ERROR) << "GL ERROR: " << error;
    }
}
}

vh::MeshBehavior::MeshBehavior(Actor* owner, const char* name)
    : ActorBehavior(owner)
{
    ResourceSystem* resource = App::Get<ResourceSystem>();
    CHECK(resource);

    mMesh = resource->GetMesh(name);
    if (!mMesh)
    {
        LOG(WARNING) << "Could not load mesh by name " << name;
    }
}

void vh::MeshBehavior::StartPlay()
{
    mRenderer = App::Get<Renderer>();
    CHECK(mRenderer);

    if (mMesh->GetDim() == 3)
    {
        mProgramID = Utils::GetShaderProgram("SimpleShader");   // default shader
    }
    else
    {
        mProgramID = Utils::GetShaderProgram("SimpleShader4");  // homogeneous coordinates shader
    }

    mWireProgramID = Utils::GetShaderProgram("Wireframe");  // wireframe shader

    if (mProgramID == 0)
    {
        LOG(ERROR) << "Object won't be rendered, because shader program invalid";
        // there was an error linking the program, do not proceed!
        return;
    }

    // get uniform locations
    uidMVP = glGetUniformLocation(mProgramID, "uMVP");
    uidM = glGetUniformLocation(mProgramID, "uM");
    uidV = glGetUniformLocation(mProgramID, "uV");
    uidPLightNum = glGetUniformLocation(mProgramID, "uPLightNum");
    uidPLightPos = glGetUniformLocation(mProgramID, "uPLightPos");
    uidPLightInt = glGetUniformLocation(mProgramID, "uPLightInt");

    uidWireColor = glGetUniformLocation(mWireProgramID, "uColor");
    uidWireMVP = glGetUniformLocation(mWireProgramID, "uMVP");

    mMesh->AddUsage();
}

void vh::MeshBehavior::EndPlay()
{
    mMesh->ReleaseUsage();
}

void vh::MeshBehavior::Tick(uint32_t delta)
{
    const M4& projection = mRenderer->GetProjection();
    const M4& view = mRenderer->GetView();
    const M4& model = mOwner->GetTransform();

    M4 MVP = projection * view * model;

    const GLBufferInfo* glInfo = mMesh->GetBufferInfo();
    CHECK(glInfo);

    glUseProgram(mProgramID);

    // -- setup lights
    glUniform1i(uidPLightNum, mRenderer->GetPointLights().size());
    V3 lightPos[MAX_POINT_LIGHTS];
    float lightInt[MAX_POINT_LIGHTS];
    for (size_t i = 0; i < mRenderer->GetPointLights().size(); ++i)
    {
        lightPos[i] = mRenderer->GetPointLights()[i]->GetPos();
        lightInt[i] = mRenderer->GetPointLights()[i]->GetIntensity();
    }
    glUniform3fv(uidPLightPos, mRenderer->GetPointLights().size(), reinterpret_cast<GLfloat*>(lightPos));
    glUniform1fv(uidPLightInt, mRenderer->GetPointLights().size(), reinterpret_cast<GLfloat*>(lightInt));

    // -- setup transforms
    glUniformMatrix4fv(uidMVP, 1, GL_FALSE, reinterpret_cast<float*>(&MVP[0][0]));
    glUniformMatrix4fv(uidM, 1, GL_FALSE, reinterpret_cast<const float*>(&model[0][0]));
    glUniformMatrix4fv(uidV, 1, GL_FALSE, reinterpret_cast<const float*>(&view[0][0]));

    // -- bind texture
    glBindTexture(GL_TEXTURE_2D, glInfo->texture);

    // -- setup buffers
    glEnableVertexAttribArray(0);
    for (GLuint i = 0; i < glInfo->attribCount; i++) glEnableVertexAttribArray(i + 1);

    glBindBuffer(GL_ARRAY_BUFFER, glInfo->attribBuffer);

    // -- vertex positions
    glVertexAttribPointer(0, mMesh->GetDim(), GL_FLOAT, GL_FALSE, 0, (GLfloat*) 0);

    // -- attributes
    GLfloat *offset = 0;
    for (GLuint i = 0; i < glInfo->attribCount; i++)
    {
        glVertexAttribPointer(i + 1, 3, GL_FLOAT, GL_FALSE, 0, offset + mMesh->GetVertexCount() * (mMesh->GetDim() + 3 * i));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glInfo->indexBuffer);
    glDrawElements(GL_TRIANGLES, glInfo->indexBufferSize, GL_UNSIGNED_INT, (void*) 0);

    glDisableVertexAttribArray(0);
    for (GLuint i = 0; i < glInfo->attribCount; i++) glDisableVertexAttribArray(i + 1);

    // -- optionally, draw wireframes
    if (mRenderer->IsOn(RenderFlags::DRAW_WIREFRAMES))
    {
        glUseProgram(mWireProgramID);

        V3 wireColor(0, 0.4f, 0);
        glUniform3fv(uidWireColor, 1, reinterpret_cast<GLfloat*>(&wireColor));
        glUniformMatrix4fv(uidWireMVP, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&MVP[0][0]));

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, glInfo->attribBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glInfo->indexBuffer);
        glDrawElements(GL_LINE_STRIP, glInfo->indexBufferSize, GL_UNSIGNED_INT, (void*) 0);
        glDisableVertexAttribArray(0);
    }
}

bool vh::MeshBehavior::IsValid()
{
    GLuint size;
    GLuint* data;
    mMesh->GetIndexData(size, data);
    return size > 0;
}
