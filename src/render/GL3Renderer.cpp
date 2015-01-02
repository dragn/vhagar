#include "GL3Renderer.hpp"
#include <glog/logging.h>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLES3/gl3.h>

static const GLfloat v_data[] = {
  -1.0f, -1.0f, 0.0f,
  1.0f, -1.0f, 0.0f,
  0.0f,  1.0f, 0.0f,
};

GLuint vertexbuffer;
GLuint programID;
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
glm::mat4 MVP;

std::string readFile(std::string filename) {
  std::ifstream ifs(filename);
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  return buffer.str();
}

GL3Renderer::GL3Renderer(SDL_Window *window) : window(window) {
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(v_data), &v_data, GL_STATIC_DRAW);

  // Creating shaders
  GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  GLint result;
  int infoLogLength;
  char buffer[256];

  LOG(INFO) << "Compiling vertex shader";
  const char *vertexShaderCode = readFile("src/shader/VertexShader.glsl").c_str();
  glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
  glCompileShader(vertexShaderID);

  glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
  glGetShaderInfoLog(vertexShaderID, 256, &infoLogLength, buffer);
  LOG(ERROR) << buffer;

  LOG(INFO) << "Compiling fragment shader";
  const char *fragmentShaderCode = readFile("src/shader/FragmentShader.glsl").c_str();
  glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
  glCompileShader(fragmentShaderID);

  glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
  glGetShaderInfoLog(fragmentShaderID, 256, &infoLogLength, buffer);
  LOG(ERROR) << buffer;

  LOG(INFO) << "Linking program";
  programID = glCreateProgram();
  glAttachShader(programID, vertexShaderID);
  glAttachShader(programID, fragmentShaderID);
  glLinkProgram(programID);

  glGetProgramiv(programID, GL_COMPILE_STATUS, &result);
  glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
  glGetProgramInfoLog(programID, 256, &infoLogLength, buffer);
  LOG(ERROR) << buffer;

  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);

  Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  Model = glm::mat4(1.0f);
}

void
GL3Renderer::render(Scene *scene) {

  Camera cam = scene->camera;
  View = glm::lookAt(cam.position,
      cam.position + cam.direction,
      glm::vec3(0,1,0));
  
  MVP = Projection * View * Model;

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(programID);
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(0);

  SDL_GL_SwapWindow(window);
}
