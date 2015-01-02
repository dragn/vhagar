#pragma once

#include "Common.hpp"
#include "Controllable.hpp"

class PlayerController {
  public:
    void control(Controllable *object);
    void release();
    void handleEvent(SDL_Event *event);
    void tick(Uint32 delta);

  private:
    Controllable *_object = nullptr;

    void handleKey(Uint32 type, SDL_KeyboardEvent *event);
    void handleMouse(Sint32 xrel, Sint32 yrel);
    bool _pressed[128];
};
