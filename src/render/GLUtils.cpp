#include "GLUtils.hpp"
#include "../core/Common.hpp"
#include <fstream>
#include <sstream>
#include <string>

void compileShader(GLuint shaderID, const char* filename) {
  char buffer[256];
  int infoLogLength;

  // ???
  LOG(INFO) << "Compiling " << filename;
  std::ifstream ifs(filename);
  std::stringstream ss;
  ss << ifs.rdbuf();

  std::string code_string = ss.str();
  const char* code = code_string.c_str();

  glShaderSource(shaderID, 1, &code, NULL);
  glCompileShader(shaderID);

  glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
  glGetShaderInfoLog(shaderID, 256, &infoLogLength, buffer);
  if (buffer[0]) LOG(FATAL) << "Error compiling shader:\n" << buffer;
}

namespace GLUtils {

  GLuint compileProgram(const char* vertexShader, const char* fragmentShader) {
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    compileShader(vertexShaderID, vertexShader);
    compileShader(fragmentShaderID, fragmentShader);

    LOG(INFO) << "Linking program";
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    char buffer[256];
    int infoLogLength;

    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    glGetProgramInfoLog(programID, 256, &infoLogLength, buffer);
    if (buffer[0]) LOG(FATAL) << buffer;

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
  }

  GLuint bufferData(GLsizeiptr size, const GLfloat *data) {
    LOG(INFO) << "Allocating buffer of size: " << size;
    GLuint id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    return id;
  }

  GLuint loadTexture(const char *filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    SDL_Surface *tex = SDL_LoadBMP(filename);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex->w, tex->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);
    SDL_FreeSurface(tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
  }

  void putUniformMat4(GLuint programID, const char *name, M4 &data) {
    GLuint id = glGetUniformLocation(programID, name);
    glUniformMatrix4fv(id, 1, GL_FALSE, &data[0][0]);
  }

  void putUniformVec3(GLuint programID, const char *name, V3 &data) {
    GLuint id = glGetUniformLocation(programID, name);
    glUniform3fv(id, 1, &data[0]);
  }
}
