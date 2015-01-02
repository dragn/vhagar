#pragma once

#include "../core/Common.hpp"
#include "Renderer.hpp"
#include <GLES3/gl3.h>
#include <SDL2/SDL.h>

class GL3Renderer : public Renderer {

  public:
    GL3Renderer(SDL_Window *window);
    ~GL3Renderer();

    void prepare(Scene *scene);
    void render(Scene *scene);
    void release();

  private:
    SDL_Window *window;

    GLuint objCount;
    GLuint *vertexDataBuffers = nullptr;
    GLuint *colorDataBuffers = nullptr;
    M4 *models = nullptr;
};
