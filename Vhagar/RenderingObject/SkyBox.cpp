#include "SkyBox.hpp"

#include "Mesh.hpp"
#include <glm/gtx/transform.hpp>

using namespace Vhagar;

const GLfloat vertexData[] = {-1, -1, -1, -1, -1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1, -1, 1, -1, 1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, -1, -1, 1, -1, -1, 1, 1, -1, 1, -1, -1, -1, -1, 1, -1, -1, 1, -1, -1, -1, 1, -1, 1, 1, -1, 1, 1, 1, -1, -1, 1, 1, 1, -1};
const GLuint vertexDataSize = 72;
const GLuint indexData[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 0, 2, 19, 3, 5, 20, 6, 8, 21, 9, 11, 22, 12, 14, 23, 15, 17};
const GLuint indexDataSize = 36;

void SkyBox::beforeRender() {
  if (isReadyToRender) {
    LOG(WARNING) << "Attempt to prepare already prepared object!";
    return;
  }

  skyTexture = loadCubeMapTexture(cubeMap);
  programID = getShaderProgram("SkyDome");

  glInfo.attribCount = 1;
  glInfo.attribBufferSize = vertexDataSize;
  glInfo.attribBuffer = bufferData(vertexDataSize, vertexData);

  glInfo.indexBufferSize = indexDataSize;
  glInfo.indexBuffer = bufferElementArray(indexDataSize, indexData);

  isReadyToRender = true;
}

void SkyBox::afterRender() {
  if (isReadyToRender) {
    GLuint ids[] = { glInfo.indexBuffer, glInfo.attribBuffer };
    glDeleteBuffers(2, ids);
    isReadyToRender = false;
  }
}

void SkyBox::render(glm::mat4 projection, glm::mat4 view) {

  if (!isReadyToRender) return;

  glDisable(GL_CULL_FACE);
  glDepthMask(GL_FALSE);
  glUseProgram(programID);

  glm::mat4 MVP = projection * view * glm::scale(M4(1.0f), V3(50.f, 50.f, 50.f));
  putUniformMat4(programID, "MVP", MVP);

  glBindBuffer(GL_ARRAY_BUFFER, glInfo.attribBuffer);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glInfo.indexBuffer);
  glDrawElements(GL_TRIANGLES, glInfo.indexBufferSize, GL_UNSIGNED_INT, (void*) 0);

  glDisableVertexAttribArray(0);
  glDepthMask(GL_TRUE);
  glEnable(GL_CULL_FACE);
}
