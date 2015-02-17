#pragma once

#include "../Common.hpp"
#include <string>
#include <map>

namespace Vhagar {
  struct CubeMap {
    std::string pos_x;
    std::string neg_x;
    std::string pos_y;
    std::string neg_y;
    std::string pos_z;
    std::string neg_z;
  };

  GLuint bufferData(GLsizeiptr size, const GLfloat *data);
  GLuint bufferElementArray(GLsizeiptr size, const GLuint *data);
  GLuint bufferTexture2D(SDL_Surface*);
  SDL_Surface *loadImage(const std::string &filename);
  GLuint loadTexture(const std::string &filename);
  GLuint loadCubeMapTexture(const CubeMap &skyBox);

  void putUniformMat4(GLuint programID, const std::string &name, M4 &data);
  void putUniformVec3(GLuint programID, const std::string &name, V3 &data);
  void putUniformFloat(GLuint programID, const std::string &name, float data);

  // Get shader program specified by key (compiles on first access)
  GLuint getShaderProgram(const std::string &key);
}
