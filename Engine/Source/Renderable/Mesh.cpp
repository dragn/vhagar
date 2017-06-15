#include "Common.hpp"

#include "Renderer/Renderer.hpp"
#include "App/App.hpp"
#include "Utils/GLUtils.hpp"
#include "Mesh.hpp"

#include <algorithm>

using namespace vh;

Mesh::~Mesh()
{
    if (IsLoaded())
    {
        LOG(ERROR) << "Not all rendered usage of mesh was released!";
    }

    if (mIndexSize > 0) delete[] mIndexData;
    if (mVertexCount > 0) delete[] mAttribData;

    if (mTexDta != nullptr)
    {
        SafeDeleteArray(mTexDta);
        mTexH = 0;
        mTexW = 0;
    }
}

void vh::Mesh::SetTexture(GLuint* rgbData, GLsizei texW, GLsizei texH)
{
    mTexDta = rgbData;
    mTexW = texW;
    mTexH = texH;

    LOG(INFO) << "Added texture data " << texW << "x" << texH;
}

void vh::Mesh::GetTexture(GLuint*& rgbaData, GLsizei& width, GLsizei& height) const
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
    }
}

GLuint vh::Mesh::GetDim() const
{
    return mDim;
}

GLuint vh::Mesh::GetAttribDataSize() const
{
    return mVertexCount * (mDim + mAttribCount * 3);
}

const vh::GLBufferInfo* vh::Mesh::GetBufferInfo() const
{
    return &mGLInfo;
}

bool vh::Mesh::DoLoad()
{
    LOG(INFO) << "Mesh load " << this;

    CHECK(mGLInfo.attribBuffer == 0);
    CHECK(mGLInfo.indexBuffer == 0);
    CHECK(mGLInfo.texture == 0);

    if (mDim == 4)
    {
        mShaderId = Utils::GetShaderProgram("SimpleShader4");
    }
    else
    {
        mShaderId = Utils::GetShaderProgram("SimpleShader");
    }

    if (mShaderId < 0) return false;

    // specify sizes
    mGLInfo.attribCount = mAttribCount;
    mGLInfo.attribBufferSize = GetAttribDataSize();
    mGLInfo.indexBufferSize = mIndexSize;

    // buffer index data
    glGenBuffers(1, &mGLInfo.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLInfo.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mGLInfo.indexBufferSize * sizeof(GLuint), mIndexData, GL_STATIC_DRAW);

    // buffer attribute data
    glGenBuffers(1, &mGLInfo.attribBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mGLInfo.attribBuffer);
    glBufferData(GL_ARRAY_BUFFER, mGLInfo.attribBufferSize * sizeof(GLfloat), mAttribData, GL_STATIC_DRAW);

    // -- load texture
    if (mTexDta != nullptr)
    {
        glGenTextures(1, &mGLInfo.texture);
        glBindTexture(GL_TEXTURE_2D, mGLInfo.texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTexW, mTexH, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTexDta);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        LOG(INFO) << "Texture loaded with ID: " << mGLInfo.texture;
    }

    return true;
}

bool vh::Mesh::DoUnload()
{
    LOG(INFO) << "Mesh unload " << this;

    GLuint ids[] = { mGLInfo.indexBuffer, mGLInfo.attribBuffer };
    glDeleteBuffers(2, ids);
    glDeleteTextures(1, &mGLInfo.texture);

    mGLInfo.indexBuffer = 0;
    mGLInfo.indexBufferSize = 0;
    mGLInfo.attribBuffer = 0;
    mGLInfo.attribBufferSize = 0;
    mGLInfo.texture = 0;

    return true;
}
