#pragma once

#include "Common.hpp"
#include "Arm.hpp"
#include "Controllable.hpp"
#include "Scene.hpp"

/**
 * PlayerController handles input events from human.
 * PlayerController has a camera, that specifies what the human sees.
 */
class PlayerController {
  public:

    // Take possession of the object
    void control(Object *object);

    // Release previously possessed object
    void release();

    void handleEvent(SDL_Event *event);
    void tick(Uint32 delta);

    Object camera;
    Arm cameraBoom;

    float cameraTurnSpeed = 0.1;
    Scene *scene = NULL;

  private:
    Object *_object = nullptr;

    void _updateCameraPos();
    void _handleKey(Uint32 type, SDL_KeyboardEvent *event);
    void _handleMouse(Sint32 xrel, Sint32 yrel);
    bool _pressed[128];
};
