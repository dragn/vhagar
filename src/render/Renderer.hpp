#pragma once

#include "../core/Scene.hpp"

class Renderer {

  public:
    virtual void render(Scene *scene) = 0;
};
