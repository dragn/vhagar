#pragma once

#include "../core/Object.hpp"

/**
 * An object that reads its mesh from .obj file
 **/
class Wavefront : public Object {
  public:
    Wavefront(const char *filename) {
      _mesh = uptr<Mesh>(new ObjMesh(filename));
    }
};
