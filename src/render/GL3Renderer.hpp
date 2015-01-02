#pragma once

#include "Renderer.hpp"
#include <SDL2/SDL.h>

class GL3Renderer : public Renderer {

  public:
    GL3Renderer(SDL_Window *window);
    void render(Scene *scene);

  private:
    SDL_Window *window;
};
