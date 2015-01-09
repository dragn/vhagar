#pragma once

#include "../core/Common.hpp"
#include "../core/Scene.hpp"
#include <SDL2/SDL.h>

/**
 * OpenGL 3 renderer
 **/
class GL3Renderer {

  public:
    GL3Renderer(SDL_Window *window);

    // Prepares new scene for rendering.
    // Should only be called once per scene before rendering.
    void prepare(const Scene &scene);

    // Renders the scene to the screen, using specified camera. (call from main loop)
    void render(const Scene &scene, const Object &camera);

  private:
    SDL_Window *window;

    std::vector<M4*> models;

    struct bufferInfo {
      GLuint attribBuffer = 0;
      GLsizei attribSize = 0;
      GLuint indexBuffer = 0;
      GLsizei indexSize = 0;
    };

    std::vector<bufferInfo> buffers;
};

