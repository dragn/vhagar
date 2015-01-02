#pragma once

#include <SDL2/SDL.h>
#include "../core/Scene.hpp"

class Application {
public:
  void run();
  Scene scene;
private:
  SDL_Window *window;
  Uint32 windowID;
  bool handleEvents();
  bool handleEvent(SDL_Event *event);
  void handleKey(Uint32 type, SDL_KeyboardEvent *event);
  void handleMouse(Sint32 xrel, Sint32 yrel);
  void tick(Uint32 millis);
};
