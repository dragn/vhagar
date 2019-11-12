#pragma once

#include "Modules/Renderer3D_GL/ResourceTypes/Resource_GL.hpp"
#include "Modules/Renderer3D_GL/RenderBuffersHandler.hpp"
#include "Modules/ResourceSystem/MResourceSystem.hpp"
#include "Modules/standard.hpp"

namespace vh
{

class BMesh;

/*
    Internal mesh representation
*/
class RMesh_GL : public Resource_GL
{
public:

    struct Payload
    {
        GLuint progId;
        glm::vec3 translate;
        glm::vec3 scale;
        glm::quat rotate;
        GLBufferInfo info;
        GLuint dim;
        GLsizei vertexCount;
        BMesh* owner;
        bool ignoreDepth;
    };

    virtual ~RMesh_GL();

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

    const GLBufferInfo* GetBufferInfo() const;

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
    GLBufferInfo mGLInfo;

    // -- size of the vertex pos vector,
    // defaults to 3, 4 for homogeneous coordinates
    GLuint mDim = 3;

    // buffer mesh to GPU memory
    virtual bool DoLoad() override;

    // release mesh from GPU memory
    virtual bool DoUnload() override;

    GLint mShaderId = -1;
};

template<> bool MResourceSystem::Load(const char* path, std::shared_ptr<RMesh_GL> mesh);
template<> bool MResourceSystem::Save(const char* path, std::shared_ptr<const RMesh_GL> mesh);

static_assert(sizeof(RMesh_GL::Payload) <= RenderBufferConstants::PAYLOAD_SIZE, "invalid payload size");

} // namespace vh
