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
    GLuint id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    return id;
  }
}
