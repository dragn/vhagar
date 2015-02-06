#pragma once

#include "Common.hpp"
#include "Utils.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include "../render/Mesh.hpp"

/**
 * Object is something that could be placed in Scene,
 * something with position and rotation, but not necessarily drawable or movable.
 */

class Object {
  public:

    Object();

    /* Position setter */
    void pos(V3 pos);

    /* Position getter */
    V3 pos() const;

    /* Scale setter */
    void scale(V3 scale);

    /* Scale getter */
    V3 scale() const;

    V3 forward() const;
    V3 up() const;

    Rot rot() const;
    void rotate(Rot rot);
    void orient(Rot rot);

    void recalcModel();

    M4 modelMatrix;

    bool hasMesh() const { return !!_mesh; }
    const Mesh *mesh() const { return _mesh.get(); }

  private:
    V3 _pos = V3(0, 0, 0);
    V3 _scale = V3(1, 1, 1);

    float _yaw = 0;
    float _pitch = 0;
    M4 _rot = M4(1.0f);

  protected:
    uptr<Mesh> _mesh;
};