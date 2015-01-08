#pragma once

#include "../core/Common.hpp"
#include <string>
#include <map>

namespace GLUtils {
  GLuint bufferData(GLsizeiptr size, const GLfloat *data);
  GLuint bufferElementArray(GLsizeiptr size, const GLuint *data);
  GLuint loadTexture(const char *filename);
  void putUniformMat4(GLuint programID, const char *name, M4 &data);
  void putUniformVec3(GLuint programID, const char *name, V3 &data);

  // Get shader program specified by key (compiles on first access)
  GLuint getShaderProgram(const std::string &key);
}
