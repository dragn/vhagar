#pragma once

#include "Modules/Renderer/Renderable/Renderable.hpp"
#include "Modules/Renderer3D_GL/RenderBuffer.hpp"
#include "Modules/standard.hpp"

namespace vh
{

class Overlay : public Renderable
{
    friend class OverlayBehavior;

public:
    struct Payload
    {
        GLuint progId;
        V4 bounds;
        GLuint texId;
        GLuint vertexBuffer;
    };

    ~Overlay();

    void SetPos(uint32_t x, uint32_t y);
    void SetTexture(SDL_Surface *surf);

protected:
    virtual bool DoLoad() override;
    virtual bool DoUnload() override;

private:
    uint32_t mPosX = 0;
    uint32_t mPosY = 0;

    GLuint mVertexBuffer = 0;
    GLuint mShaderId = 0;

    GLfloat mVertices[18];
    V4 mBounds;
    GLuint mTexId = 0;

    void _UpdateVertices(uint32_t w, uint32_t h);
};

static_assert(sizeof(Overlay::Payload) <= RenderBufferConstants::PAYLOAD_SIZE, "invalid payload size");

} // namespace vh
