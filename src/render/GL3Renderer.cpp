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

GL3Renderer::GL3Renderer(SDL_Window *window) : window(window), objCount(0) {
  programID = GLUtils::compileProgram("src/shader/VertexShader.glsl", "src/shader/FragmentShader.glsl");

  Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

  // Enable backface culling
  glEnable(GL_CULL_FACE);
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
}

void
GL3Renderer::prepare(Scene *scene) {
  auto *objs = scene->objects();
  objCount = objs->size();

  vertexDataBuffers = uptr<GLuint[]>(new GLuint[objCount]);
  colorDataBuffers = uptr<GLuint[]>(new GLuint[objCount]);
  models = uptr<M4[]>(new M4[objCount]);

  Drawable *obj;

  for (int i = 0; i < objCount; i++) {
    obj = objs->at(i).get();
    vertexDataBuffers[i] = GLUtils::bufferData(obj->vertexDataSize(), obj->vertexData());
    colorDataBuffers[i] = GLUtils::bufferData(obj->colorDataSize(), obj->colorData());
    models[i] = glm::translate(glm::scale(M4(1.0f), obj->scale()), obj->pos());
  }

  uvBuffer = GLUtils::bufferData(sizeof(uvData), uvData);
  textureID = GLUtils::loadTexture("images/dice.bmp");
}

void
GL3Renderer::render(Scene *scene) {

  Object *cam = &scene->camera;
  M4 View = glm::lookAt(cam->pos(), cam->pos() + cam->forward(), V3(0, 1, 0));

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(programID);

  for (int i = 0; i < objCount; i++) {
    M4 MVP = Projection * View * models[i];
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexDataBuffers[i]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorDataBuffers[i]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
  }

  SDL_GL_SwapWindow(window);
}
