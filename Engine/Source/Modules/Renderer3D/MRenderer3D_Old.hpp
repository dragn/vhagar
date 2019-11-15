#pragma once

#include "Modules/Debug/TextOverlay.hpp"
#include "Modules/Renderer3D_GL/Behaviors/BPointLight_GL.hpp"
#include "Modules/Renderer3D_GL/RenderBuffersHandler.hpp"
#include "Modules/Renderer3D_GL/GLResources/GLMesh.hpp"
#include "Modules/Renderer3D_GL/GLResources/GLSkyBox.hpp"
#include "Modules/Renderer3D_GL/GLResources/Lights.hpp"
#include "Modules/Renderer3D_GL/GLResources/GLResource.hpp"
#include "Modules/Renderer3D_GL/GLResources/Overlay.hpp"
#include "Modules/Renderer3D/RendererOptions.hpp"
#include "Modules/standard.hpp"

namespace vh
{

    const static size_t MAX_POINT_LIGHTS = 10;
    const static size_t MAX_AMBIENT_LIGHTS = 1;
    const static size_t MAX_DIRECTION_LIGHTS = 10;

    namespace RenderFlags
    {
        enum Type
        {
            DRAW_WIREFRAMES = 1,
            DRAW_LABELS = 1 << 1,
        };
    }

    struct RenderTask
    {
        enum class Action
        {
            Load,
            Release
        } action;

        std::shared_ptr<GLResource> renderable;
    };

    /*
          //            \\
        MMMM Renderer3D MMMM
          \\            //

        OpenGL-based 3d renderer implementation
    */
    class MRenderer3D_Old : public vh::Module
    {
        VH_MODULE(MRenderer3D_Old);

    public:

        MRenderer3D_Old(const RendererOptions& opts)
            : Module(TickFrequency::NORMAL)
            , mOptions(opts)
            , mStatOverlay(10, 10, 100, 20)
        {}
        virtual ~MRenderer3D_Old() {}

        virtual Ret TickInit(uint32_t delta) override;
        virtual Ret TickClose(uint32_t delta) override;

        virtual void StartFrame() override;
        virtual void EndFrame() override;

        const RendererOptions& GetOptions() const
        {
            return mOptions;
        }

        uint8_t GetFlags() const
        {
            return mFlags;
        }

        bool IsOn(RenderFlags::Type flag) const
        {
            return mFlags && flag;
        }

        void Toggle(RenderFlags::Type flag)
        {
            mFlags ^= flag;
        }

    private:
        RendererOptions mOptions;

        uint8_t mFlags = 0;

        SDL_GLContext mGLContext;
        SDL_Window *mWindow;
        uint32_t mWindowID;

        glm::mat4 mProjection;

        RenderBufferHandler mBufferHandler;

        GLuint mWireProgramID;

        std::thread mRenderThread;
        static void RenderThreadStatic(MRenderer3D_Old* renderer);
        void RenderThread();
        bool mRenderThreadExit = false;
        bool mRenderThreadReady = false;
        bool mRenderThreadStarted = false;

        void DoInit();
        void DoRender(const RenderBuffer& last, const RenderBuffer& cur, float factor);
        void DoRenderMesh(glm::mat4 view, glm::mat4 projection, const GLMesh::Payload* payload, const std::vector<GLPointLight::Payload>& lights);
        void DoRenderSkyBox(glm::mat4 view, glm::mat4 projection, const GLSkyBox::Payload& payload);
        void DoRenderOverlay(const Overlay::Payload& payload);

        void AddLoadTask(std::shared_ptr<GLResource> renderable);
        void AddReleaseTask(std::shared_ptr<GLResource> renderable);

        std::mutex mTaskQueueLock;
        std::queue<RenderTask> mTaskQueue;

        void HandleTasks();

        uint32_t mLastFPSReport = 0;
        uint32_t mFrameCount = 0;

        TextOverlay mStatOverlay;
    };

} // namespace vh
