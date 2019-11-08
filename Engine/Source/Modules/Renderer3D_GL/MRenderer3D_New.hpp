#pragma once

#include "Modules/Renderer/RendererOptions.hpp"
#include "Modules/Renderer/MRenderer3D.hpp"
#include "Modules/Renderer3D_GL/Renderer3D_New_Thread.hpp"

namespace vh
{

    class MRenderer3D_New : public MRenderer3D
    {
        VH_MODULE(MRenderer3D_New);
        VH_PROPERTY_RO(RendererOptions, Options);

    public:
        MRenderer3D_New(const RendererOptions& opts)
            : MRenderer3D(TickFrequency::NEVER)
            , mOptions(opts)
        {}
        virtual ~MRenderer3D_New() {}

        virtual Ret TickInit(uint32_t delta) override;
        virtual Ret TickClose(uint32_t delta) override;

        virtual void StartFrame() override;
        virtual void EndFrame() override;

        RenderBuffer& GetWriteBuffer();

    private:
        SDL_GLContext mGLContext;
        SDL_Window* mWindow;
        uint32_t mWindowID;

        glm::mat4 mProjection;

        MRenderer3D_New_Thread mRenderThread;
    };

} // namespace vh
