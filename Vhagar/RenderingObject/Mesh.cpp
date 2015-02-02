#include "Mesh.hpp"
#include "../Renderer/GLUtils.hpp"

using namespace Vhagar;

void
Mesh::beforeRender() {
  if (isReadyToRender) {
    LOG(WARNING) << "Attempt to prepare already prepared object!";
    return;
  }
  
  programID = getShaderProgram("SimpleShader");
  
  if (programID == 0) {
    LOG(ERROR) << "Object won't be rendered, because shader program invalid";
    // there was an error linking the program, do not proceed!
    return;
  }
  
  // specify sizes
  glInfo.attribBufferSize = _attribSize;
  glInfo.indexBufferSize = _indexSize;

  // buffer index data
  glInfo.indexBuffer = bufferElementArray(glInfo.indexBufferSize, _indexData.get());

  // buffer attribute data
  glGenBuffers(1, &glInfo.attribBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, glInfo.attribBuffer);

  GLuint bytes = glInfo.attribBufferSize * sizeof(GLfloat);
  glBufferData(GL_ARRAY_BUFFER, 5 * bytes, NULL, GL_STATIC_DRAW);

  glBufferSubData(GL_ARRAY_BUFFER, 0 * bytes, bytes, _vertexData.get());
  glBufferSubData(GL_ARRAY_BUFFER, 1 * bytes, bytes, _normalData.get());
  glBufferSubData(GL_ARRAY_BUFFER, 2 * bytes, bytes, _aColorData.get());
  glBufferSubData(GL_ARRAY_BUFFER, 3 * bytes, bytes, _dColorData.get());
  glBufferSubData(GL_ARRAY_BUFFER, 4 * bytes, bytes, _sColorData.get());

  programID = getShaderProgram("SimpleShader");

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
  glm::mat4 MVP = projection * view * model;
  
  glUseProgram(programID);

  V3 lightPos = V3(0, 30, 0);
  putUniformVec3(programID, "LightPosition_worldspace", lightPos);

  putUniformMat4(programID, "MVP", MVP);
  putUniformMat4(programID, "M", model);
  putUniformMat4(programID, "V", view);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);

  glBindBuffer(GL_ARRAY_BUFFER, glInfo.attribBuffer);

  GLfloat *offset = 0;
  GLuint size = glInfo.attribBufferSize;
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, offset);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, offset + size);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, offset + 2 * size);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, offset + 3 * size);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, offset + 4 * size);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glInfo.indexBuffer);
  glDrawElements(GL_TRIANGLES, glInfo.indexBufferSize, GL_UNSIGNED_INT, (void*)0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);
}
