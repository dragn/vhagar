#include "PlayerController.hpp"

void
PlayerController::control(Object *object) {
  _object = object;
  for (bool &b : _pressed) b = false;
  cameraBoom.length = 20;
  _updateCameraPos();
}

void
PlayerController::handleEvent(SDL_Event *event) {

  if (_object == nullptr) return;

  switch (event->type) {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      _handleKey(event->type, &event->key);
      break;

    case SDL_MOUSEMOTION:
      _handleMouse(event->motion.xrel, event->motion.yrel);
      break;
  }
}

void
PlayerController::_handleKey(Uint32 type, SDL_KeyboardEvent *event) {
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
PlayerController::_updateCameraPos() {
  if (_object == nullptr) return;

  cameraBoom.pos(_object->pos() + V3(0, 0, -4));
  camera.pos(cameraBoom.pos() - cameraBoom.forward() * (float) cameraBoom.length);

  camera.orient(cameraBoom.rot());
}

void
PlayerController::_handleMouse(Sint32 xrel, Sint32 yrel) {
  // if some object is possessed turn camera around it
  if (_object == nullptr) return;

  cameraBoom.rotate(Rot(- xrel * cameraTurnSpeed, - yrel * cameraTurnSpeed, 0.0f)); 

  // Attach camera boom to object
  _updateCameraPos();
}

void
PlayerController::tick(Uint32 delta) {
  if (_object == nullptr) return;
  float sec = delta / 1000.0f;
  if (_pressed['w']) {
    _updateCameraPos();
  }
  //  if (_pressed['s']) _object->moveForward(-sec);
  //  if (_pressed['a']) _object->moveRight(-sec);
  //  if (_pressed['d']) _object->moveRight(sec);

}
