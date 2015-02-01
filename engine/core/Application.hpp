#pragma once

#include "Common.hpp"
#include "../core/Scene.hpp"
#include "PlayerController.hpp"

class Application {
  public:
    static const Uint32 SCREEN_WIDTH = 1024;
    static const Uint32 SCREEN_HEIGHT = 768;
    void run();

    virtual void OnStart() {};

  private:
    SDL_Window *window;
    Uint32 windowID = -1;
    bool handleEvents();
    bool handleEvent(SDL_Event *event);
    void tick(Uint32 millis);

    PlayerController playerController;
};
