#pragma once

#include "Modules/Renderer3D_GL/GLBuffers.hpp"
#include "Modules/Renderer3D_GL/GLResources/GLResource.hpp"
#include "Modules/Renderer3D_GL/RenderBuffer.hpp"
#include "Modules/ResourceSystem/Resource.hpp"
#include "Modules/standard.hpp"

namespace vh
{

    class GLSkyBox : public GLResource
    {
        friend class MRenderer3D_GL_Thread;

        VH_PROPERTY_RW(Utils::CubeMap, CubeMap);

    public:
        struct Payload
        {
            GLuint progId;
            GLBuffers info;
        };

        GLSkyBox() = default;
        GLSkyBox(const Utils::CubeMap& cubeMap) : mCubeMap(cubeMap) {}

        void SetupPayload(GLSkyBox::Payload& payload);

    protected:
        virtual bool DoLoad() override;
        virtual bool DoUnload() override;

    private:
        GLBuffers mGLInfo;
        GLuint mShaderId;
    };

    static_assert(sizeof(GLSkyBox::Payload) <= RenderBufferConstants::PAYLOAD_SIZE, "invalid payload size");

} // namespace vh
