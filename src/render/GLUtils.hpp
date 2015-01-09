#pragma once

#include "../core/Common.hpp"
#include <string>
#include <map>

namespace GLUtils {
  GLuint bufferData(GLsizeiptr size, const GLfloat *data);
  GLuint bufferElementArray(GLsizeiptr size, const GLuint *data);
  GLuint loadTexture(const std::string &filename);
  GLuint loadCubeMapTexture(
      const std::string &px,
      const std::string &nx,
      const std::string &py,
      const std::string &ny,
      const std::string &pz,
      const std::string &nz);

  void putUniformMat4(GLuint programID, const std::string &name, M4 &data);
  void putUniformVec3(GLuint programID, const std::string &name, V3 &data);
  void putUniformFloat(GLuint programID, const std::string &name, float data);

  // Get shader program specified by key (compiles on first access)
  GLuint getShaderProgram(const std::string &key);
}
