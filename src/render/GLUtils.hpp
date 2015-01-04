#pragma once

#include <GLES3/gl3.h>

namespace GLUtils {
  GLuint compileProgram(const char *vertexShader, const char *fragmentShader);  
  GLuint bufferData(GLsizeiptr size, const GLfloat *data);
  GLuint loadTexture(const char *filename);
}
