#include "GL3Renderer.hpp"
#include <glog/logging.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLUtils.hpp"

const GLfloat uvData[] = {
  891/1920, 97/1080,
  0.000103f, 1.0f-0.336048f,
  0.335973f, 1.0f-0.335903f,
  1.000023f, 1.0f-0.000013f,
  0.667979f, 1.0f-0.335851f,
  0.999958f, 1.0f-0.336064f,
  0.667979f, 1.0f-0.335851f,
  0.336024f, 1.0f-0.671877f,
  0.667969f, 1.0f-0.671889f,
  1.000023f, 1.0f-0.000013f,
  0.668104f, 1.0f-0.000013f,
  0.667979f, 1.0f-0.335851f,
  891/1920, 97/1080,
  0.335973f, 1.0f-0.335903f,
  0.336098f, 1.0f-0.000071f,
  0.667979f, 1.0f-0.335851f,
  0.335973f, 1.0f-0.335903f,
  0.336024f, 1.0f-0.671877f,
  1.000004f, 1.0f-0.671847f,
  0.999958f, 1.0f-0.336064f,
  0.667979f, 1.0f-0.335851f,
  0.668104f, 1.0f-0.000013f,
  0.335973f, 1.0f-0.335903f,
  0.667979f, 1.0f-0.335851f,
  0.335973f, 1.0f-0.335903f,
  0.668104f, 1.0f-0.000013f,
  0.336098f, 1.0f-0.000071f,
  0.000103f, 1.0f-0.336048f,
  0.000004f, 1.0f-0.671870f,
  0.336024f, 1.0f-0.671877f,
  0.000103f, 1.0f-0.336048f,
  0.336024f, 1.0f-0.671877f,
  0.335973f, 1.0f-0.335903f,
  0.667969f, 1.0f-0.671889f,
  1.000004f, 1.0f-0.671847f,
  0.667979f, 1.0f-0.335851f
};

GLuint uvBuffer;
GLuint programID;
GLuint textureID;

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
  auto *objs = scene->objects();

  buffers.clear();
  models.clear();

  const Mesh *obj;
  _bufferNames bn;

  // Buffer all objects in the scene
  for (size_t i = 0; i < objs->size(); i++) {
    if (objs->at(i)->hasMesh()) {

      obj = objs->at(i)->mesh();

      bn.vertexSize = obj->_vertexDataSize;
      bn.indexSize = obj->_indexDataSize;

      bn.indexBuf = GLUtils::bufferElementArray(obj->_indexDataSize, obj->_indexData.get());
      bn.vertexBuf = GLUtils::bufferData(obj->_vertexDataSize, obj->_vertexData.get());
      bn.normalBuf = GLUtils::bufferData(obj->_normalDataSize, obj->_normalData.get());
      bn.aColorBuf = GLUtils::bufferData(obj->_aColorDataSize, obj->_aColorData.get());    
      bn.dColorBuf = GLUtils::bufferData(obj->_dColorDataSize, obj->_dColorData.get());    
      bn.sColorBuf = GLUtils::bufferData(obj->_sColorDataSize, obj->_sColorData.get());    

      models.push_back(&objs->at(i)->modelMatrix);
      buffers.push_back(bn);
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
  _bufferNames bn;

  for (int i = 0; i < buffers.size(); i++) {
    bn = buffers[i];

    MVP = Projection * View * *(models[i]);
    GLUtils::putUniformMat4(programID, "MVP", MVP);
    GLUtils::putUniformMat4(programID, "M", *(models[i]));
    GLUtils::putUniformMat4(programID, "V", View);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    
    glBindBuffer(GL_ARRAY_BUFFER, bn.vertexBuf);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, bn.normalBuf);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, bn.aColorBuf);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, bn.dColorBuf);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, bn.sColorBuf);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bn.indexBuf);
    glDrawElements(GL_TRIANGLES, bn.indexSize, GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
  }

  SDL_GL_SwapWindow(window);
}
