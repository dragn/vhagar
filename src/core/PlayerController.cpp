#include "PlayerController.hpp"

void
PlayerController::control(Controllable *object) {
  _object = object;
  for (bool &b : _pressed) b = false;
}

void
PlayerController::handleEvent(SDL_Event *event) {

  if (_object == nullptr) return;
  
  switch (event->type) {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      handleKey(event->type, &event->key);
      break;

    case SDL_MOUSEMOTION:
      handleMouse(event->motion.xrel, event->motion.yrel);
      break;
  }
}

void
PlayerController::handleKey(Uint32 type, SDL_KeyboardEvent *event) {
  SDL_Keycode k = event->keysym.sym;
  if (k > 127) return; // handle only symbol keys for now...
  switch (type) {
    case SDL_KEYDOWN:
      _pressed[k] = true;
      break;
    case SDL_KEYUP:
      _pressed[k] = false;
      break;
  }
}

void
PlayerController::handleMouse(Sint32 xrel, Sint32 yrel) {
  _object->turnRight(xrel);
}

void
PlayerController::tick(Uint32 delta) {
  if (_object == nullptr) return;
  float sec = delta / 1000.0f;
  if (_pressed['w']) _object->moveForward(sec);
  if (_pressed['s']) _object->moveForward(-sec);
  if (_pressed['a']) _object->moveRight(-sec);
  if (_pressed['d']) _object->moveRight(sec);
}
