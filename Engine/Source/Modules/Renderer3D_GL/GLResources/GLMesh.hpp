#pragma once

#include "Modules/Renderer3D_GL/GLBuffers.hpp"
#include "Modules/Renderer3D_GL/GLResources/GLResource.hpp"
#include "Modules/Renderer3D_GL/RenderBuffersHandler.hpp"
#include "Modules/ResourceSystem/MResourceSystem.hpp"
#include "Modules/standard.hpp"

namespace vh
{

class BMesh;

/*
    Internal mesh representation
*/
class GLMesh : public GLResource, public Resource
{
public:

    struct Payload
    {
        GLuint progId;
        glm::vec3 translate;
        glm::vec3 scale;
        glm::quat rotate;
        GLBuffers info;
        GLuint dim;
        GLsizei vertexCount;
        BMesh* owner;
        bool ignoreDepth;
    };

    virtual ~GLMesh();

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

    const GLBuffers GetBufferInfo() const;

    GLint GetShaderId() const { return mShaderId; }

private:
    GLuint mVertexCount = 0;
    GLuint mAttribCount = 0;
    GLfloat *mAttribData = nullptr;

    GLuint mIndexSize = 0;
    GLuint *mIndexData = nullptr;

    GLuint* mTexDta = nullptr;
    GLsizei mTexW = 0;
    GLsizei mTexH = 0;

    // render context buffers
    GLBuffers mGLInfo;

    // -- size of the vertex pos vector,
    // defaults to 3, 4 for homogeneous coordinates
    GLuint mDim = 3;

    // buffer mesh to GPU memory
    virtual bool DoLoad() override;

    // release mesh from GPU memory
    virtual bool DoUnload() override;

    GLint mShaderId = -1;
};

template<> bool MResourceSystem::Load(const char* path, std::shared_ptr<GLMesh> mesh);
template<> bool MResourceSystem::Save(const char* path, std::shared_ptr<const GLMesh> mesh);

static_assert(sizeof(GLMesh::Payload) <= RenderBufferConstants::PAYLOAD_SIZE, "invalid payload size");

} // namespace vh
