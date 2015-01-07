#pragma once

#include <SDL2/SDL.h>
#include "../core/Scene.hpp"
#include "PlayerController.hpp"

class Application {
  public:
    void run();
  private:
    SDL_Window *window;
    Uint32 windowID = -1;
    bool handleEvents();
    bool handleEvent(SDL_Event *event);
    void tick(Uint32 millis);

    PlayerController playerController;
};
