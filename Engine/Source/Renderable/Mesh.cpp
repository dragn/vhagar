#include "Common.hpp"

#include "Renderer/Renderer.hpp"
#include "App/App.hpp"
#include "Utils/GLUtils.hpp"
#include "Mesh.hpp"

#include <algorithm>

using namespace vh;

Mesh::~Mesh()
{
    LOG(INFO) << "Mesh destructor";
    if (mIndexSize > 0) delete[] mIndexData;
    if (mAttribSize > 0) delete[] mAttribData;
    if (mTexture != NULL) SDL_FreeSurface(mTexture);
}

void
Mesh::BeforeRender()
{
    if (isReadyToRender)
    {
        LOG(WARNING) << "Attempt to prepare already prepared object!";
        return;
    }

    if (mIndexSize == 0)
    {
        LOG(WARNING) << "Object has no data!";
        return;
    }

    mProgramID = Utils::GetShaderProgram("SimpleShader");
    mWireProgramID = Utils::GetShaderProgram("Wireframe");

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

    // specify sizes
    mGLInfo.attribBufferSize = mAttribSize;
    mGLInfo.indexBufferSize = mIndexSize;

    // buffer index data
    mGLInfo.indexBuffer = Utils::BufferElementArray(mGLInfo.indexBufferSize, mIndexData);

    // buffer attribute data
    glGenBuffers(1, &mGLInfo.attribBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mGLInfo.attribBuffer);

    GLuint bytes = mGLInfo.attribBufferSize * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, mAttribCount * bytes, NULL, GL_STATIC_DRAW);

    for (size_t i = 0; i < mAttribCount; i++)
    {
        glBufferSubData(GL_ARRAY_BUFFER, i * bytes, bytes, mAttribData + i * mAttribSize);
    }

    // load texture
    if (mTexture != NULL)
    {
        mGLInfo.texture = Utils::BufferTexture2D(mTexture);
        LOG(INFO) << "Texture loaded with ID: " << mGLInfo.texture;
    }

    mRenderer = App::Get<Renderer>();

    isReadyToRender = true;
}

void
Mesh::AfterRender()
{
    if (isReadyToRender)
    {
        GLuint ids[] = { mGLInfo.indexBuffer, mGLInfo.attribBuffer };
        glDeleteBuffers(2, ids);
        isReadyToRender = false;
    }
    if (mGLInfo.texture > 0)
    {
        LOG(INFO) << "Deleting previous texture: " << mGLInfo.texture;
        glDeleteTextures(1, &mGLInfo.texture);
    }
}

/*
    TODO optimize: get uniform location in BeforeRender, optimize lights array cache
*/
void
Mesh::Render(glm::mat4 projection, glm::mat4 view, const Renderer* renderer)
{
    if (!isReadyToRender) return;

    M4 MVP = projection * view * mModel;

    glUseProgram(mProgramID);

    glUniform1i(uidPLightNum, renderer->GetPointLights().size());

    V3 lightPos[MAX_POINT_LIGHTS];
    float lightInt[MAX_POINT_LIGHTS];
    for (size_t i = 0; i < renderer->GetPointLights().size(); ++i)
    {
        lightPos[i] = renderer->GetPointLights()[i]->GetPos();
        lightInt[i] = renderer->GetPointLights()[i]->GetIntensity();
    }
    glUniform3fv(uidPLightPos, renderer->GetPointLights().size(), reinterpret_cast<GLfloat*>(lightPos));
    glUniform1fv(uidPLightInt, renderer->GetPointLights().size(), reinterpret_cast<GLfloat*>(lightInt));

    glUniformMatrix4fv(uidMVP, 1, GL_FALSE, reinterpret_cast<float*>(&MVP[0][0]));
    glUniformMatrix4fv(uidM, 1, GL_FALSE, reinterpret_cast<float*>(&mModel[0][0]));
    glUniformMatrix4fv(uidV, 1, GL_FALSE, reinterpret_cast<float*>(&view[0][0]));

    if (mGLInfo.texture)
    {
        glBindTexture(GL_TEXTURE_2D, mGLInfo.texture);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    for (GLuint i = 0; i < mAttribCount; i++)
    {
        glEnableVertexAttribArray(i);
    }

    glBindBuffer(GL_ARRAY_BUFFER, mGLInfo.attribBuffer);

    GLfloat *offset = 0;
    for (GLuint i = 0; i < mAttribCount; i++)
    {
        glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, offset + i * mGLInfo.attribBufferSize);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLInfo.indexBuffer);
    glDrawElements(GL_TRIANGLES, mGLInfo.indexBufferSize, GL_UNSIGNED_INT, (void*) 0);

    for (GLuint i = 0; i < mAttribCount; i++)
    {
        glDisableVertexAttribArray(i);
    }
    /*
    if (mRenderer->IsOn(RenderFlags::DRAW_WIREFRAMES))
    {
        MVP = projection * view * mModel;

        glUseProgram(mWireProgramID);
        Utils::PutUniformVec3(mWireProgramID, "uColor", V3(0, 0.4f, 0));
        Utils::PutUniformMat4(mWireProgramID, "uMVP", MVP);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, mGLInfo.attribBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLInfo.indexBuffer);
        glDrawElements(GL_LINE_STRIP, mGLInfo.indexBufferSize, GL_UNSIGNED_INT, (void*) 0);
        glDisableVertexAttribArray(0);
    }
    */
}

void Mesh::SetTexture(const std::string &filename)
{
    if (mGLInfo.texture > 0)
    {
        LOG(INFO) << "Deleting previous texture: " << mGLInfo.texture;
        glDeleteTextures(1, &mGLInfo.texture);
        SDL_FreeSurface(mTexture);
    }

    LOG(INFO) << "Setting up texture " << filename;
    mTexture = Utils::LoadImage(filename);
}

void vh::Mesh::SetTexture(const SDL_Surface* texture)
{
    if (mGLInfo.texture > 0)
    {
        LOG(INFO) << "Deleting previous texture: " << mGLInfo.texture;
        glDeleteTextures(1, &mGLInfo.texture);
    }

    if (mTexture != nullptr)
    {
        SDL_FreeSurface(mTexture);
        mTexture = nullptr;
    }

    if (texture != nullptr) mTexture = new SDL_Surface(*texture);
}
