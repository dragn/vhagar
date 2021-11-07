#pragma once

#include "Modules/Renderer3D/MRenderer3D.hpp"
#include "RenderBuffer.hpp"
#include "GLResources/GLResource.hpp"
#include "GLResources/GLMesh.hpp"
#include "GLResources/GLSkyBox.hpp"
#include "GLResources/GLOverlay.hpp"
#include "Behaviors/BPointLight_GL.hpp"

namespace vh
{

    class MRenderer3D_GL : public MRenderer3D
    {
    public:
        MRenderer3D_GL(const RendererOptions& opts)
            : MRenderer3D(opts)
        {
            SetTickStep(static_cast<int32_t>(TickFrequency::EACH));
        }

        virtual Ret TickInit(uint32_t delta) override;
        virtual Ret TickRun(uint32_t delta) override;
        virtual Ret TickClose(uint32_t delta) override;

        virtual void StartFrame() override;
        virtual void EndFrame() override;

        void UpdateCameraPos();

        virtual void HandleEvent(SDL_Event* event) override;

        RenderBuffer& GetWriteBuffer() { return mRenderBuffers.GetNextBuffer(); }

        void LoadRes(std::shared_ptr<GLResource> const& res);
        void UnloadRes(std::shared_ptr<GLResource> const& res);

    private:
        Ret InitSDL();
        void ShutdownSDL();

        Ret InitOpenGL();
        void ShutdownOpenGL();

        Ret InitImGUI();
        void ShutdownImGUI();

        void RenderAll();

        void DoRender(const RenderBuffer& last, const RenderBuffer& cur, float factor);
        void DoRenderMesh(glm::mat4 view, glm::mat4 projection, const GLMesh::Payload* payload, const std::vector<GLPointLight::Payload>& lights);
        void DoRenderSkyBox(glm::mat4 view, glm::mat4 projection, const GLSkyBox::Payload& payload);
        void DoRenderOverlay(const GLOverlay::Payload& payload);

        void HandleFrameCount();

        void ProcessLoadQueue();
        void ProcessUnloadQueue();

    private:
        SDL_Window* mWindow = nullptr;
        SDL_GLContext mGLContext = 0;
        glm::mat4 mProjection;

        RenderBufferHandler mRenderBuffers;

        float mFPS = 0.0f;
        uint32_t mLastFPSReport = 0;
        uint32_t mFrameCount = 0;

        cs::CritSection mLoadQueueCS;
        std::queue<std::shared_ptr<GLResource>> mLoadQueue;
        std::queue<std::shared_ptr<GLResource>> mUnloadQueue;
    };

}