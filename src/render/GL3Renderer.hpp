#pragma once

#include "../core/Common.hpp"
#include "Renderer.hpp"
#include <GLES3/gl3.h>
#include <SDL2/SDL.h>

class GL3Renderer : public Renderer {

  public:
    GL3Renderer(SDL_Window *window);

    void prepare(Scene *scene);
    void render(Scene *scene);

  private:
    SDL_Window *window;

    GLuint objCount;
    uptr<GLuint[]> vertexDataBuffers;
    uptr<GLuint[]> vertexDataSizes;
    uptr<GLuint[]> normalDataBuffers;
    uptr<GLuint[]> normalDataSizes;
    uptr<GLuint[]> colorDataBuffers;
    uptr<GLuint[]> colorDataSizes;
    uptr<M4[]> models;
};
