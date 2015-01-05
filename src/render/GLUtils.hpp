#pragma once

#include "../core/Common.hpp"

namespace GLUtils {
  GLuint compileProgram(const char *vertexShader, const char *fragmentShader);  
  GLuint bufferData(GLsizeiptr size, const GLfloat *data);
  GLuint loadTexture(const char *filename);
  void putUniformMat4(GLuint programID, const char *name, M4 &data);
  void putUniformVec3(GLuint programID, const char *name, V3 &data);
}
