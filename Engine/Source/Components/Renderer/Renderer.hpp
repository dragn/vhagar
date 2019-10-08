#pragma once

#include <queue>
#include <thread>
#include <vector>
#include <forward_list>
#include "Renderable/Renderable.hpp"
#include "App/Component.hpp"
#include "RenderBuffersHandler.hpp"
#include "Renderable/Mesh.hpp"
#include "Renderable/SkyBox.hpp"
#include "Renderable/Lights.hpp"
#include "Debug/TextOverlay.hpp"

namespace vh {

const static size_t MAX_POINT_LIGHTS = 10;
const static size_t MAX_AMBIENT_LIGHTS = 1;
const static size_t MAX_DIRECTION_LIGHTS = 10;

/**
 * Renderer options
 */
struct RendererOptions {
    const char *resourceRoot = ".";
    int screenWidth = 1024;
    int screenHeight = 768;
    bool borderless = true;

    enum
    {
        AA_OFF = 0, // Antialiasing turned off
        AA_2X,      // MSAAx2
        AA_4X       // MSAAx4
    } antialias = AA_OFF;

    enum
    {
        MON_FIRST = 0,
        MON_SECOND
    } monitor = MON_FIRST;
};

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

    std::shared_ptr<Renderable> renderable;
};

/**
 * The Renderer
 **/
class Renderer : public vh::Component {
    VH_COMPONENT(Renderer);

public:

    Renderer(const RendererOptions& opts)
        : Component(TickFrequency::NEVER)
        , mOptions(opts)
        , mStatOverlay(10, 10, 100, 20)
    {}
    virtual ~Renderer() {}

    virtual Ret TickInit(uint32_t delta) override;
    virtual Ret TickClose(uint32_t delta) override;

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

    // Get writable render buffer
    RenderBufferHandler& GetBufferHandler()
    {
        return mBufferHandler;
    }

    void AddLoadTask(std::shared_ptr<Renderable> renderable);
    void AddReleaseTask(std::shared_ptr<Renderable> renderable);

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
    static void RenderThreadStatic(Renderer* renderer);
    void RenderThread();
    bool mRenderThreadExit = false;
    bool mRenderThreadReady = false;
    bool mRenderThreadStarted = false;

    void DoInit();
    void DoRender(const RenderBuffer* last, const RenderBuffer* cur, float factor);
    void DoRenderMesh(glm::mat4 view, glm::mat4 projection, const Mesh::Payload* payload, const std::vector<PointLight::Payload>& lights);
    void DoRenderSkyBox(glm::mat4 view, glm::mat4 projection, const SkyBox::Payload& payload);
    void DoRenderOverlay(const Overlay::Payload& payload);

    std::mutex mTaskQueueLock;
    std::queue<RenderTask> mTaskQueue;

    void HandleTasks();

    uint32_t mLastFPSReport = 0;
    uint32_t mFrameCount = 0;

    TextOverlay mStatOverlay;
};

}
