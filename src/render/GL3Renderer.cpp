#include "GL3Renderer.hpp"
#include <glog/logging.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLUtils.hpp"

GLuint programID;

glm::mat4 Projection;

GL3Renderer::GL3Renderer(SDL_Window *window) : window(window) {
  programID = GLUtils::compileProgram("src/shader/VertexShader.glsl", "src/shader/FragmentShader.glsl");

  Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

  // Enable backface culling
  glDisable(GL_CULL_FACE);
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
GL3Renderer::prepare(Scene *scene) {

  for (auto &info : buffers) {
    if (info.attribBuffer > 0) glDeleteBuffers(1, &info.attribBuffer);
    if (info.indexBuffer > 0) glDeleteBuffers(1, &info.indexBuffer);
  }

  models.clear();

  const Mesh *mesh;
  bufferInfo info;

  // Buffer all objects in the scene
  for (auto obj : *scene->objects()) {
    if (obj->hasMesh()) {

      mesh = obj->mesh();

      // specify sizes
      info.attribSize = mesh->_attribSize;
      info.indexSize = mesh->_indexSize;

      // buffer index data
      info.indexBuffer = GLUtils::bufferElementArray(info.indexSize, mesh->_indexData.get());

      // buffer attribute data
      GLuint buf;
      glGenBuffers(1, &buf);
      glBindBuffer(GL_ARRAY_BUFFER, buf);

      GLuint bytes = info.attribSize * sizeof(GLfloat);
      glBufferData(GL_ARRAY_BUFFER, 5 * bytes, NULL, GL_STATIC_DRAW);

      glBufferSubData(GL_ARRAY_BUFFER, 0 * bytes, bytes, mesh->_vertexData.get());
      glBufferSubData(GL_ARRAY_BUFFER, 1 * bytes, bytes, mesh->_normalData.get());
      glBufferSubData(GL_ARRAY_BUFFER, 2 * bytes, bytes, mesh->_aColorData.get());
      glBufferSubData(GL_ARRAY_BUFFER, 3 * bytes, bytes, mesh->_dColorData.get());
      glBufferSubData(GL_ARRAY_BUFFER, 4 * bytes, bytes, mesh->_sColorData.get());

      info.attribBuffer = buf;

      buffers.push_back(info);

      // Store model matrix pointers
      models.push_back(&obj->modelMatrix);
    }
  }

  LOG(INFO) << "objCount: " << buffers.size();
  //uvBuffer = GLUtils::bufferData(sizeof(uvData), uvData);
  //  textureID = GLUtils::loadTexture("images/dice.bmp");
}

void
GL3Renderer::render(Scene *scene, Object *camera) {

  M4 View = glm::lookAt(camera->pos(), camera->pos() + camera->forward(), V3(0, 1, 0));

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(programID);

  V3 lightPos = V3(0, 100, 0);
  GLUtils::putUniformVec3(programID, "LightPosition_worldspace", lightPos);

  M4 MVP;
  bufferInfo info;
  GLuint bytes;

  for (int i = 0; i < buffers.size(); i++) {
    info = buffers[i];

    MVP = Projection * View * *(models[i]);
    GLUtils::putUniformMat4(programID, "MVP", MVP);
    GLUtils::putUniformMat4(programID, "M", *(models[i]));
    GLUtils::putUniformMat4(programID, "V", View);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    bytes = info.attribSize * sizeof(GLfloat);

    glBindBuffer(GL_ARRAY_BUFFER, info.attribBuffer);
    void *offset = 0;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, offset);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, offset + bytes);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, offset + 2 * bytes);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, offset + 3 * bytes);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, offset + 4 * bytes);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, info.indexBuffer);
    glDrawElements(GL_TRIANGLES, info.indexSize, GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
  }

  SDL_GL_SwapWindow(window);
}
