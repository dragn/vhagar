#pragma once

#include <SDL2/SDL.h>
#include "../core/Scene.hpp"
#include "PlayerController.hpp"

class Application {
public:
  void run();
  Scene scene;
private:
  SDL_Window *window;
  Uint32 windowID;
  bool handleEvents();
  bool handleEvent(SDL_Event *event);
  void tick(Uint32 millis);

  PlayerController playerController;
};
