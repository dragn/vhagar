#pragma once

#include "Drawable.hpp"
#include "Controllable.hpp"

class Scene {

  public:
    Scene();

    Controllable camera;

    const std::vector<Drawable> objects() { return _objects; };

  private:
    std::vector<Drawable> _objects;
};
