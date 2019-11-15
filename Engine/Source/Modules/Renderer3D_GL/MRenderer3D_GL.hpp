#pragma once

#include "Modules/Renderer3D/RendererOptions.hpp"
#include "Modules/Renderer3D/MRenderer3D.hpp"
#include "Modules/Renderer3D_GL/Renderer3D_GL_Thread.hpp"
#include "Modules/Debug/TextOverlay.hpp"

namespace vh
{

    class MRenderer3D_GL : public MRenderer3D
    {
    public:
        MRenderer3D_GL(const RendererOptions& opts)
            : MRenderer3D(opts)
            , mStatOverlay(5, 5, 400, 40)
        {
            SetTickStep(static_cast<int32_t>(TickFrequency::NORMAL));
        }

        virtual ~MRenderer3D_GL() {}

        virtual Ret TickInit(uint32_t delta) override;
        virtual Ret TickRun(uint32_t delta) override;
        virtual Ret TickClose(uint32_t delta) override;

        virtual void StartFrame() override;
        virtual void EndFrame() override;

        RenderBuffer& GetWriteBuffer();

        void LoadRes(std::shared_ptr<GLResource> const& res);
        void UnloadRes(std::shared_ptr<GLResource> const& res);

    private:
        Ret InitSDL();

    private:
        SDL_Window* mWindow;

        glm::mat4 mProjection;
        TextOverlay mStatOverlay;

        MRenderer3D_GL_Thread mRenderThread;
    };

} // namespace vh
