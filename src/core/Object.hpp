#pragma once

#include "Common.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

/**
 * Object is something that could be placed in Scene,
 * something with position and rotation, but not necessarily drawable or movable.
 */

class Object {
  public:

    /* Position setter */
    void pos(V3 pos) { _pos = pos; };

    /* Position getter */
    const V3 pos() { return _pos; };

    /* Rotation setters */
    void yaw(float yaw) { _yaw = yaw; };

    /* Rotation getters */
    float yaw() { return _yaw; };

    V3 forward() {
      return glm::rotateY(V3(0, 0, -1), - _yaw);
    };

private:
    V3 _pos;
    float _yaw;
};
