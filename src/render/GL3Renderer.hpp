#pragma once

#include "../core/Common.hpp"
#include "../core/Scene.hpp"
#include <GLES3/gl3.h>
#include <SDL2/SDL.h>

struct _bufferNames {
  GLuint indexBuf;
  GLuint vertexBuf;
  GLuint normalBuf;
  GLuint aColorBuf;
  GLuint dColorBuf;
  GLuint sColorBuf;

  GLsizei indexSize;
  GLsizei vertexSize;
};

class GL3Renderer {

  public:
    GL3Renderer(SDL_Window *window);

    void prepare(Scene *scene);
    void render(Scene *scene, Object *camera);

  private:
    SDL_Window *window;

    std::vector<M4*> models;
    std::vector<_bufferNames> buffers;
};
