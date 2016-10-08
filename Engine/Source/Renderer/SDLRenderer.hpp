#pragma once

#include <forward_list>
#include "Renderable/Renderable.hpp"
#include "LightSource.hpp"
#include "App/Component.hpp"

namespace vh {

/**
 * SDLRenderer options
 */
struct SDLRendererOptions {
    const char *resourceRoot = ".";
    size_t screenWidth = 1024;
    size_t screenHeight = 768;
};

/**
 * The SDLRenderer
 **/
class SDLRenderer : public vh::Component {

public:
    SDLRenderer(const std::string& name) : Component(name, 16) {}
    virtual ~SDLRenderer() {}

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
    void SetLightSources(std::vector<LightSource> lightSources)
    {
        mLightSources = lightSources;
    }

    /**
     * Renders all object using specified View matrix. (call from main loop)
     */
    void Render();

    /**
     * Sets up a projection matrix
     */
    void SetProjection(glm::mat4 projection) { mProjection = projection; };

    /**
     * Sets up view matrix (camera)
     */
    void SetView(glm::mat4 view) { mView = view; };

private:
    SDLRendererOptions mOptions;

    SDL_GLContext mGLContext;
    SDL_Window *mWindow;
    uint32_t mWindowID;

    std::vector<LightSource> mLightSources;

    std::forward_list<Renderable*> mObjects;

    glm::mat4 mProjection;
    glm::mat4 mView;

    // Multisampling-related framebuffer and textures
    GLuint fbo, tex, depth_tex;

    bool mMultisample = false;

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
