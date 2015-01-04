#pragma once

#include "../drawable/Drawable.hpp"
#include "Controllable.hpp"

class Scene {

  public:
    Controllable camera;

    std::vector<uptr<Drawable>> *objects() { return &_objects; };

  protected:
    std::vector<uptr<Drawable>> _objects;
};
