#include "Common.hpp"

#include "Renderer/Renderer.hpp"
#include "App/App.hpp"
#include "Utils/GLUtils.hpp"
#include "Mesh.hpp"

#include <algorithm>

using namespace vh;

vh::Mesh::Mesh()
    : mTexDta(nullptr)
    , mTexW(0)
    , mTexH(0)
{
}

Mesh::~Mesh()
{
    LOG(INFO) << "Mesh destructor";
    if (mIndexSize > 0) delete[] mIndexData;
    if (mVertexNum > 0) delete[] mAttribData;

    if (mTexDta != nullptr)
    {
        SafeDeleteArray(mTexDta);
        mTexH = 0;
        mTexW = 0;
    }
}

void vh::Mesh::SetTexture(uint32_t* rgbData, size_t texW, size_t texH)
{
    if (mTexDta != nullptr || mGLInfo.texture != 0)
    {
        LOG(ERROR) << "Texture is already set";
        return;
    }

    mTexDta = rgbData;
    mTexW = texW;
    mTexH = texH;

    LOG(INFO) << "Added texture data " << texW << "x" << texH;

    if (isReadyToRender) BufferTexture();
}


void vh::Mesh::GetTexture(uint32_t*& rgbaData, size_t& width, size_t& height) const
{
    rgbaData = mTexDta;
    width = mTexW;
    height = mTexH;
}


void vh::Mesh::SetDim(GLuint dim)
{
    if (dim != 3 && dim != 4)
    {
        LOG(ERROR) << "Unsupported dimensions: " << dim;
        return;
    }

    if (dim != mDim)
    {
        mDim = dim;

        if (isReadyToRender)
        {
            // reload buffers
            AfterRender();
            BeforeRender();
        }
    }
}


GLuint vh::Mesh::GetDim() const
{
    return mDim;
}

GLuint vh::Mesh::GetAttribDataSize() const
{
    return mVertexNum * (mDim + mAttribCount * 3);
}

void vh::Mesh::BufferTexture()
{
    if (mGLInfo.texture != 0)
    {
        LOG(WARNING) << "Attempt to buffer texture second time";
        return;
    }

    if (mTexDta != nullptr)
    {
        glGenTextures(1, &mGLInfo.texture);
        glBindTexture(GL_TEXTURE_2D, mGLInfo.texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTexW, mTexH, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTexDta);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        LOG(INFO) << "Texture loaded with ID: " << mGLInfo.texture;
    }
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

    if (mDim == 3)
    {
        mProgramID = Utils::GetShaderProgram("SimpleShader");   // default shader
    }
    else
    {
        mProgramID = Utils::GetShaderProgram("SimpleShader4"); // homogeneous coordinates shader
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

    // specify sizes
    mGLInfo.attribBufferSize = GetAttribDataSize();
    mGLInfo.indexBufferSize = mIndexSize;

    // buffer index data
    mGLInfo.indexBuffer = Utils::BufferElementArray(mGLInfo.indexBufferSize, mIndexData);

    // buffer attribute data
    glGenBuffers(1, &mGLInfo.attribBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mGLInfo.attribBuffer);

    GLuint bytes = mGLInfo.attribBufferSize * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, bytes, mAttribData, GL_STATIC_DRAW);

    // -- load texture
    BufferTexture();

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
    glDeleteTextures(1, &mGLInfo.texture);
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

    // -- setup lights
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

    // -- setup transforms
    glUniformMatrix4fv(uidMVP, 1, GL_FALSE, reinterpret_cast<float*>(&MVP[0][0]));
    glUniformMatrix4fv(uidM, 1, GL_FALSE, reinterpret_cast<float*>(&mModel[0][0]));
    glUniformMatrix4fv(uidV, 1, GL_FALSE, reinterpret_cast<float*>(&view[0][0]));

    // -- bind texture
    glBindTexture(GL_TEXTURE_2D, mGLInfo.texture);

    // -- setup buffers
    glEnableVertexAttribArray(0);
    for (GLuint i = 0; i < mAttribCount; i++) glEnableVertexAttribArray(i + 1);

    glBindBuffer(GL_ARRAY_BUFFER, mGLInfo.attribBuffer);

    // -- vertex positions
    glVertexAttribPointer(0, mDim, GL_FLOAT, GL_FALSE, 0, (GLfloat*) 0);

    // -- attributes
    GLfloat *offset = 0;
    for (GLuint i = 0; i < mAttribCount; i++)
    {
        glVertexAttribPointer(i + 1, 3, GL_FLOAT, GL_FALSE, 0, offset + mVertexNum * (mDim + 3 * i));
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLInfo.indexBuffer);
    glDrawElements(GL_TRIANGLES, mGLInfo.indexBufferSize, GL_UNSIGNED_INT, (void*) 0);

    glDisableVertexAttribArray(0);
    for (GLuint i = 0; i < mAttribCount; i++) glDisableVertexAttribArray(i + 1);

    // -- optionally, draw wireframes
    if (mRenderer->IsOn(RenderFlags::DRAW_WIREFRAMES))
    {
        MVP = projection * view * mModel;

        glUseProgram(mWireProgramID);

        V3 wireColor(0, 0.4f, 0);
        glUniform3fv(uidWireColor, 1, reinterpret_cast<GLfloat*>(&wireColor));
        glUniformMatrix4fv(uidWireMVP, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&MVP[0][0]));

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, mGLInfo.attribBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLInfo.indexBuffer);
        glDrawElements(GL_LINE_STRIP, mGLInfo.indexBufferSize, GL_UNSIGNED_INT, (void*) 0);
        glDisableVertexAttribArray(0);
    }
}
