#pragma once

#include "Object.hpp"
#include <GLES3/gl3.h>

class Drawable : public Object {

  public:
    GLsizeiptr vertexDataSize;
    const GLfloat *vertexData;

    GLsizeiptr colorDataSize;
    const GLfloat *colorData;
};
