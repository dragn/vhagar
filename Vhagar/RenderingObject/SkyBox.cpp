#include "SkyBox.hpp"

#include "Mesh.hpp"
#include <glm/gtx/transform.hpp>

using namespace Vhagar;

void SkyBox::beforeRender() {
  if (isReadyToRender) {
    LOG(WARNING) << "Attempt to prepare already prepared object!";
    return;
  }

  ObjMesh skyMesh("models/skybox.obj");

  skyTexture = loadCubeMapTexture(cubeMap);
  skyProgram = getShaderProgram("SkyDome");

  skyMeshBuf = bufferData(skyMesh._attribSize, skyMesh._vertexData.get());
  skyMeshIdxBuf = bufferElementArray(skyMesh._indexSize, skyMesh._indexData.get());
  skyMeshSize = skyMesh._indexSize;

  isReadyToRender = true;
}

void SkyBox::afterRender() {
  if (isReadyToRender) {
    GLuint ids[] = { skyMeshBuf, skyMeshIdxBuf };
    glDeleteBuffers(2, ids);
    isReadyToRender = false;
  } 
}

void SkyBox::render(glm::mat4 projection, glm::mat4 view) {
  glDisable(GL_CULL_FACE);
  glDepthMask(GL_FALSE);
  glUseProgram(skyProgram);
  
  glm::mat4 MVP = projection * view * glm::scale(M4(1.0f), V3(50.f, 50.f, 50.f));
  putUniformMat4(skyProgram, "MVP", MVP);

  glBindBuffer(GL_ARRAY_BUFFER, skyMeshBuf);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyMeshIdxBuf);
  glDrawElements(GL_TRIANGLES, skyMeshSize, GL_UNSIGNED_INT, (void*) 0);

  glDisableVertexAttribArray(0);
  glDepthMask(GL_TRUE);
  glEnable(GL_CULL_FACE);
}
