#pragma once

#include "../core/Common.hpp"
#include "Drawable.hpp"

class Box : public Drawable {

  public:
    Box();

    virtual GLsizeiptr vertexDataSize() {
      return _vertexDataSize;
    }

    virtual GLfloat *vertexData() {
      return _vertexData.get();
    }

    virtual GLsizeiptr colorDataSize() {
      return _colorDataSize;
    }

    virtual GLfloat *colorData() {
      return _colorData.get();
    }

  private:
    GLsizeiptr _vertexDataSize;
    uptr<GLfloat[]> _vertexData;

    GLsizeiptr _colorDataSize;
    uptr<GLfloat[]> _colorData;
};
