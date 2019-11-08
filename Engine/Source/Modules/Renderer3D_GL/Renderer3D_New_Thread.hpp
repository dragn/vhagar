#pragma once

#include "RenderBuffersHandler.hpp"
#include "Modules/Renderer/Renderable/Mesh.hpp"
#include "Modules/Renderer/Renderable/SkyBox.hpp"
#include "Modules/Renderer/Renderable/Overlay.hpp"
#include "Modules/Renderer/Renderable/Lights.hpp"

namespace vh
{

    class MRenderer3D_New_Thread
    {
    public:
        void Start();
        void Stop(bool wait = true);

        bool IsReady() const { return mReady.load(); }
        bool IsStarted() const { return mStarted.load(); }

        RenderBuffer& GetWriteBuffer();
        void FlipBuffers();

    private:
        vh::Thread mThread;

        void Run();

        void DoInit();
        void DoRender(const RenderBuffer* last, const RenderBuffer* cur, float factor);
        void DoRenderMesh(glm::mat4 view, glm::mat4 projection, const Mesh::Payload* payload, const std::vector<PointLight::Payload>& lights);
        void DoRenderSkyBox(glm::mat4 view, glm::mat4 projection, const SkyBox::Payload& payload);
        void DoRenderOverlay(const Overlay::Payload& payload);

        std::atomic<bool> mReady = false;
        std::atomic<bool> mStarted = false;
        std::atomic<bool> mRequestStop = false;

        RenderBufferHandler mRenderBuffers;
    };

} // namespace vh
