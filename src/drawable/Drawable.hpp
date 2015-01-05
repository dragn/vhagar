#pragma once

#include "../core/Object.hpp"
#include "../core/Common.hpp"

class Drawable : public Object {

  friend class GL3Renderer;

  protected:
  GLsizeiptr _vertexDataSize;
  uptr<GLfloat[]> _vertexData;

  GLsizeiptr _colorDataSize;
  uptr<GLfloat[]> _colorData;

  GLsizeiptr _normalDataSize;
  uptr<GLfloat[]> _normalData;
};
