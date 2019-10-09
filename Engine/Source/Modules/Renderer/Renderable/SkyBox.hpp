#pragma once

#include "Modules/Renderer/Renderable/Renderable.hpp"
#include "Modules/Renderer/RenderBuffer.hpp"
#include "Modules/ResourceSystem/Resource.hpp"
#include "Modules/standard.hpp"

namespace vh
{

class SkyBox : public Renderable, public Resource
{
    friend class SkyBoxBehavior;

public:
    struct Payload
    {
        GLuint progId;
        GLBufferInfo info;
    };

    SkyBox(const Utils::CubeMap& cubeMap) : mCubeMap(cubeMap) {}

    virtual bool DoLoad() override;
    virtual bool DoUnload() override;

private:
    GLBufferInfo mGLInfo;
    GLuint mShaderId;
    Utils::CubeMap mCubeMap;
};

static_assert(sizeof(SkyBox::Payload) <= RenderBufferConstants::PAYLOAD_SIZE, "invalid payload size");

} // namespace vh
