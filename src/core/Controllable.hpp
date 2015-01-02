#pragma once

#include "Common.hpp"
#include "Object.hpp"

/**
 * Something that can be controlled by player or AI.
 */

class Controllable : public Object {
  
  public:

    Controllable() : moveSpeed(5), turnSpeed(0.05) {}

    float moveSpeed;
    float turnSpeed;

    void moveForward(float value) {
      pos(pos() + value * moveSpeed * forward());
    }
    void moveRight(float value) {
      V3 rightVector = glm::cross(forward(), V3(0, 1, 0));
      pos(pos() + value * moveSpeed * rightVector);
    }
    void turnRight(float value) {
      yaw(yaw() + turnSpeed * value);
    }
};
