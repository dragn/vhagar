#pragma once

#include <vector>
#include <forward_list>
#include "Renderable/Renderable.hpp"
#include "Light.hpp"
#include "App/Component.hpp"

namespace vh {

/**
 * Renderer options
 */
struct RendererOptions {
    const char *resourceRoot = ".";
    size_t screenWidth = 1024;
    size_t screenHeight = 768;
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

/**
 * The Renderer
 **/
class Renderer : public vh::Component {

public:
    static const char* COMPONENT_NAME;

    Renderer() : Component(COMPONENT_NAME, 16) {}
    Renderer(const RendererOptions& opts) : Component(COMPONENT_NAME, 16), mOptions(opts) {}
    virtual ~Renderer() {}

    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    /**
     * Add the object to the rendered world.
     */
    void AddObject(Renderable *object);

    /**
     * Remove the object from rendering
     */
    void RemoveObject(Renderable *object);

    /**
     * Set up light sources
     */
    size_t AddLight(Light* light)
    {
        mLights.push_back(light);
        return mLights.size() - 1;
    }

    void RemoveLight(const Light* light)
    {
        for (auto l = mLights.begin(); l != mLights.end();)
        { 
            if (*l == light) l = mLights.erase(l);
            else ++l;
        }
    }

    /**
     * Sets up a projection matrix
     */
    void SetProjection(glm::mat4 projection) { mProjection = projection; };

    /**
     * Sets up view matrix (camera)
     */
    void SetView(glm::mat4 view) { mView = view; };

    const RendererOptions& GetOptions() const
    {
        return mOptions;
    }

private:
    RendererOptions mOptions;

    SDL_GLContext mGLContext;
    SDL_Window *mWindow;
    uint32_t mWindowID;

    std::vector<Light*> mLights;

    std::forward_list<Renderable*> mObjects;

    glm::mat4 mProjection;
    glm::mat4 mView;

    /**
     * Override to provide custom actions before Rendering phase
     */
    virtual void BeforeRender() {};

    /**
     * Override to provide custom actions after Rendering phase
     */
    virtual void AfterRender() {};
};

}
