#pragma once

#include "Modules/Renderer3D_GL/ResourceTypes/Resource_GL.hpp"
#include "Modules/Renderer3D_GL/RenderBuffer.hpp"
#include "Modules/ResourceSystem/Resource.hpp"
#include "Modules/standard.hpp"

namespace vh
{

class RSkyBox_GL : public Resource_GL
{
    friend class SkyBoxBehavior;

public:
    struct Payload
    {
        GLuint progId;
        GLBufferInfo info;
    };

    RSkyBox_GL(const Utils::CubeMap& cubeMap) : mCubeMap(cubeMap) {}

    virtual bool DoLoad() override;
    virtual bool DoUnload() override;

private:
    GLBufferInfo mGLInfo;
    GLuint mShaderId;
    Utils::CubeMap mCubeMap;
};

static_assert(sizeof(RSkyBox_GL::Payload) <= RenderBufferConstants::PAYLOAD_SIZE, "invalid payload size");

} // namespace vh
