#pragma once

#include "../core/Object.hpp"
#include "../core/Common.hpp"

class Drawable : public Object {

  friend class GL3Renderer;

  protected:
  GLsizeiptr _vertexDataSize = 0;
  uptr<GLfloat[]> _vertexData;

  GLsizeiptr _ambientColorDataSize = 0;
  uptr<GLfloat[]> _ambientColorData;

  GLsizeiptr _diffuseColorDataSize = 0;
  uptr<GLfloat[]> _diffuseColorData;

  GLsizeiptr _specularColorDataSize = 0;
  uptr<GLfloat[]> _specularColorData;

  GLsizeiptr _normalDataSize = 0;
  uptr<GLfloat[]> _normalData;

  GLsizei _indexDataSize = 0;
  uptr<GLuint[]> _indexData;

  GLsizei _materialIndexDataSize = 0;
  uptr<GLsizei[]> _materialIndexData;
};
