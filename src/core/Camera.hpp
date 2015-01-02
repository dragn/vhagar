#pragma once

#include <glm/glm.hpp>
#include "Controllable.hpp"

class Camera : public Controllable {

  public:
    M4 view();
};
