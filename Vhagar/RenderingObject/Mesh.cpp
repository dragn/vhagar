#include "Mesh.hpp"
#include "../Renderer/GLUtils.hpp"

using namespace Vhagar;

Mesh::~Mesh() {
  LOG(INFO) << "Mesh destructor";
  delete [] indexData;
  delete [] attribData;
}

void
Mesh::beforeRender() {
  if (isReadyToRender) {
    LOG(WARNING) << "Attempt to prepare already prepared object!";
    return;
  }

  if (indexSize == 0) {
    LOG(WARNING) << "Object has no data!";
    return;
  }

  programID = getShaderProgram("SimpleShader");

  if (programID == 0) {
    LOG(ERROR) << "Object won't be rendered, because shader program invalid";
    // there was an error linking the program, do not proceed!
    return;
  }

  // specify sizes
  glInfo.attribBufferSize = attribSize;
  glInfo.indexBufferSize = indexSize;

  // buffer index data
  glInfo.indexBuffer = bufferElementArray(glInfo.indexBufferSize, indexData);

  // buffer attribute data
  glGenBuffers(1, &glInfo.attribBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, glInfo.attribBuffer);

  GLuint bytes = glInfo.attribBufferSize * sizeof(GLfloat);
  glBufferData(GL_ARRAY_BUFFER, attribCount * bytes, NULL, GL_STATIC_DRAW);

  for (size_t i = 0; i < attribCount; i++) {
    glBufferSubData(GL_ARRAY_BUFFER, i * bytes, bytes, attribData + i * attribSize);
  }

  isReadyToRender = true;
}

void
Mesh::afterRender() {
  if (isReadyToRender) {
    GLuint ids[] = { glInfo.indexBuffer, glInfo.attribBuffer };
    glDeleteBuffers(2, ids);
    isReadyToRender = false;
  }
}

void
Mesh::render(glm::mat4 projection, glm::mat4 view) {

  if (!isReadyToRender) return;

  glm::mat4 MVP = projection * view * model;

  glUseProgram(programID);

  V3 lightPos = V3(0, 1, 0);
  putUniformVec3(programID, "uLightPosition", lightPos);
  putUniformFloat(programID, "uLightIntensity", 1.0f);

  putUniformMat4(programID, "uMVP", MVP);
  putUniformMat4(programID, "uM", model);
  putUniformMat4(programID, "uV", view);

  for (size_t i = 0; i < attribCount; i++) {
    glEnableVertexAttribArray(i);
  }

  glBindBuffer(GL_ARRAY_BUFFER, glInfo.attribBuffer);

  GLfloat *offset = 0;
  for (size_t i = 0; i < attribCount; i++) {
    glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, offset + i * glInfo.attribBufferSize);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glInfo.indexBuffer);
  glDrawElements(GL_TRIANGLES, glInfo.indexBufferSize, GL_UNSIGNED_INT, (void*)0);

  for (size_t i = 0; i < attribCount; i++) {
    glDisableVertexAttribArray(i);
  }
}
