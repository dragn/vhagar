#pragma once

#include "RenderBuffersHandler.hpp"
#include "Modules/Renderer3D_GL/Behaviors/BPointLight_GL.hpp"
#include "Modules/Renderer3D_GL/GLResources/GLMesh.hpp"
#include "Modules/Renderer3D_GL/GLResources/GLSkyBox.hpp"
#include "Modules/Renderer3D_GL/GLResources/Overlay.hpp"
#include "Modules/Renderer3D_GL/GLResources/Lights.hpp"
#include "Modules/Renderer/RendererOptions.hpp"

namespace vh
{

    class MRenderer3D_GL_Thread
    {
    public:
        void Start(SDL_Window* window, const RendererOptions& options);
        void Stop(bool wait = true);

        bool IsReady() const { return mReady.load(); }
        bool IsStarted() const { return mStarted.load(); }

        RenderBuffer& GetWriteBuffer();
        void FlipBuffers();

        void LoadRes(std::shared_ptr<GLResource> const& res);
        void UnloadRes(std::shared_ptr<GLResource> const& res);

    private:
        SDL_GLContext mGLContext;
        SDL_Window* mWindow;
        RendererOptions mOptions;

        vh::Thread mThread;

        void Run();

        void HandleFrameCount();

        void DoInit();
        void DoRender(const RenderBuffer& last, const RenderBuffer& cur, float factor);
        void DoRenderMesh(glm::mat4 view, glm::mat4 projection, const GLMesh::Payload* payload, const std::vector<GLPointLight::Payload>& lights);
        void DoRenderSkyBox(glm::mat4 view, glm::mat4 projection, const GLSkyBox::Payload& payload);
        void DoRenderOverlay(const Overlay::Payload& payload);

        std::atomic<bool> mReady = false;
        std::atomic<bool> mStarted = false;
        std::atomic<bool> mRequestStop = false;

        M4 mProjection;

        RenderBufferHandler mRenderBuffers;

        uint32_t mLastFPSReport = 0;
        uint32_t mFrameCount = 0;

        cs::CritSection mLoadQueueCS;
        std::queue<std::shared_ptr<GLResource>> mLoadQueue;
        std::queue<std::shared_ptr<GLResource>> mUnloadQueue;
        void ProcessLoadQueue();
        void ProcessUnloadQueue();
    };

} // namespace vh
