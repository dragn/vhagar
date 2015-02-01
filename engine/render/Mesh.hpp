#pragma once

#include "../core/Common.hpp"

class Mesh {

  friend class GL3Renderer;

  protected:
  GLsizei _attribSize;
  GLsizei _indexSize;

  uptr<GLfloat[]> _vertexData;
  uptr<GLfloat[]> _normalData;
  uptr<GLfloat[]> _aColorData;
  uptr<GLfloat[]> _dColorData;
  uptr<GLfloat[]> _sColorData;

  uptr<GLuint[]> _indexData;
};

class ObjMesh : public Mesh {
  public:
    ObjMesh(const char *filename);
};
