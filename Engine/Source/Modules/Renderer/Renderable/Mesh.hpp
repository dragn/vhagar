#pragma once

#include "Renderable.hpp"
#include "Modules/Renderer/RenderBuffersHandler.hpp"
#include "Modules/ResourceSystem/ResourceSystem.hpp"

namespace vh
{

class MeshBehavior;

/*
    Internal mesh representation
*/
class Mesh : public Renderable, public Resource
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
        MeshBehavior* owner;
        bool ignoreDepth;
    };

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

template<> bool ResourceSystem::Load(const char* path, std::shared_ptr<Mesh> mesh);
template<> bool ResourceSystem::Save(const char* path, std::shared_ptr<const Mesh> mesh);

static_assert(sizeof(Mesh::Payload) <= RenderBufferConstants::PAYLOAD_SIZE, "invalid payload size");

} // namespace vh
