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
    void inline pos(V3 pos) { _pos = pos; };

    /* Position getter */
    const inline V3 pos() { return _pos; };

    /* Scale setter */
    void inline scale(V3 scale) { _scale = scale; };

    /* Scale getter */
    V3 inline scale() { return _scale; };

    /* Rotation setters */
    void inline yaw(float yaw) { _yaw = yaw; };

    /* Rotation getters */
    float inline yaw() { return _yaw; };

    V3 forward() {
      return glm::rotateY(V3(0, 0, -1), - _yaw);
    };

  private:
    V3 _pos = V3(0, 0, 0);
    V3 _scale = V3(1, 1, 1);
    float _yaw = 0;
};
