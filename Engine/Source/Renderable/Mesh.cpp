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
