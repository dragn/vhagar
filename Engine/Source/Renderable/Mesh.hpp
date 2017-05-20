#pragma once

#include "Renderer/Lights.hpp"
#include "Renderable.hpp"

namespace vh {

/*
    Internal mesh representation
*/
class Mesh
{
public:
    virtual ~Mesh();

    void SetAttribData(GLuint vertexCount, GLuint attribCount, GLfloat* data)
    {
        mVertexCount = vertexCount;
        mAttribCount = attribCount;
        mAttribData = data;
    }

    void SetIndexData(GLuint size, GLuint* data)
    {
        mIndexSize = size;
        mIndexData = data;
    }

    void GetAttribData(GLuint& vertexCount, GLuint& attribCount, GLfloat*& data) const
    {
        vertexCount = mVertexCount;
        attribCount = mAttribCount;
        data = mAttribData;
    }

    void GetIndexData(GLuint& size, GLuint*& data) const
    {
        size = mIndexSize;
        data = mIndexData;
    }

    GLuint GetVertexCount() { return mVertexCount; }

    void SetTexture(GLuint* rgbaData, GLsizei width, GLsizei height);
    void GetTexture(GLuint*& rgbaData, GLsizei& width, GLsizei& height) const;

    void SetDim(GLuint);
    GLuint GetDim() const;

    GLuint GetAttribDataSize() const;

private:
    GLuint mVertexCount = 0;
    GLuint mAttribCount = 0;
    GLfloat *mAttribData = nullptr;

    GLuint mIndexSize = 0;
    GLuint *mIndexData = nullptr;

    GLuint* mTexDta = nullptr;
    GLsizei mTexW = 0;
    GLsizei mTexH = 0;

    // -- size of the vertex pos vector,
    // defaults to 3, 4 for homogeneous coordinates
    GLuint mDim = 3;
};

} // namespace vh
