#pragma once

#include "../core/Object.hpp"

class SpaceShip : public Object {
  public:
    SpaceShip() {
      _mesh = uptr<Mesh>(new ObjMesh("models/SimpleShip/Spaceship.obj"));
      _mesh->pos(V3(0, 0, 20));
    }
};
