#pragma once

#include "Modules/Renderer3D_GL/GLResources/GLResource.hpp"
#include "Modules/Renderer3D_GL/RenderBuffer.hpp"
#include "Modules/standard.hpp"

namespace vh
{

class GLOverlay : public GLResource
{
    friend class BOverlay;

    VH_PROPERTY_RO(V2, Pos);
    VH_PROPERTY_RO(V2, Size);

public:
    struct Payload
    {
        GLuint progId;
        V4 bounds;
        GLuint texId;
        GLuint vertexBuffer;
    };

public:
    GLOverlay() = default;
    GLOverlay(V2 pos, V2 size) : mPos(pos), mSize(size) {}
    virtual ~GLOverlay();

    void SetupPayload(Payload& payload) const;
    void BlitTexture(SDL_Surface* surf);

protected:
    virtual bool DoLoad() override;
    virtual bool DoUnload() override;

private:
    GLuint mVertexBuffer = 0;
    GLuint mShaderId = 0;
    GLuint mTexId = 0;

    GLfloat mVertices[18];
    V4 mBounds;

    void UpdateVertices();
};

static_assert(sizeof(GLOverlay::Payload) <= RenderBufferConstants::PAYLOAD_SIZE, "invalid payload size");

} // namespace vh
