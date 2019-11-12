#pragma once

#include "Modules/Renderer/RendererOptions.hpp"
#include "Modules/Renderer/MRenderer3D.hpp"
#include "Modules/Renderer3D_GL/Renderer3D_GL_Thread.hpp"

namespace vh
{

    class MRenderer3D_GL : public MRenderer3D
    {
    public:
        MRenderer3D_GL(const RendererOptions& opts)
            : MRenderer3D(opts)
        {}

        virtual ~MRenderer3D_GL() {}

        virtual Ret TickInit(uint32_t delta) override;
        virtual Ret TickClose(uint32_t delta) override;

        virtual void StartFrame() override;
        virtual void EndFrame() override;

        RenderBuffer& GetWriteBuffer();

    private:
        SDL_Window* mWindow;
        uint32_t mWindowID;

        glm::mat4 mProjection;

        MRenderer3D_GL_Thread mRenderThread;
    };

} // namespace vh
