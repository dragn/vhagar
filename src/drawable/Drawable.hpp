#pragma once

#include "../core/Object.hpp"
#include <GLES3/gl3.h>

class Drawable : public Object {

  public:
    virtual GLsizeiptr vertexDataSize() = 0;
    virtual GLfloat *vertexData() = 0;

    virtual GLsizeiptr colorDataSize() = 0;
    virtual GLfloat *colorData() = 0;

};
