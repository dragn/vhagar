#include "UITexture.hpp"
#include "../Renderer/GLUtils.hpp"

using namespace Vhagar;

void UITexture::setTexture(SDL_Surface *s) {
  surf = s;
  if (isReadyToRender) {
    if (texId > 0) {
      glDeleteTextures(1, &texId);
      texId = -1;
    }
    if (surf != NULL) {
      texId = bufferTexture2D(surf);
    }
  }
}

void UITexture::beforeRender() {
  if (surf != NULL) {
    texId = bufferTexture2D(surf);
    vertexBuffer = bufferData(12, vertices);
  }

  programID = getShaderProgram("OSD");
  if (programID < 0) {
    LOG(INFO) << "Unable to load program OSD";
    return;
  }

  isReadyToRender = true;
}

void UITexture::afterRender() {
  if (texId > 0) {
    glDeleteTextures(1, &texId);
    texId = -1;
  }
  if (vertexBuffer >= 0) {
    glDeleteBuffers(1, &vertexBuffer);
    vertexBuffer = -1;
  }
}

void UITexture::setBounds(Rect rect) {
  GLfloat v[] = {
    -1.0f + rect.x,                1.0f - rect.y,
    -1.0f + rect.x + rect.width,   1.0f - rect.y,
    -1.0f + rect.x + rect.width,   1.0f - rect.y - rect.height,
    -1.0f + rect.x + rect.width,   1.0f - rect.y - rect.height,
    -1.0f + rect.x,                1.0f - rect.y - rect.height,
    -1.0f + rect.x,                1.0f - rect.y
  };
  std::copy(v, v + 12, vertices);
  bounds = V4(rect.x, rect.y, rect.width, rect.height);

  if (isReadyToRender) {
    if (vertexBuffer >= 0) glDeleteBuffers(1, &vertexBuffer);
    vertexBuffer = bufferData(12, vertices);
  }
}

void UITexture::render(glm::mat4 projection, glm::mat4 view) {
  if (!isReadyToRender && texId < 0) return;

  glDisable(GL_CULL_FACE);
  glUseProgram(programID);

  putUniformVec4(programID, "uBounds", bounds);

  glBindTexture(GL_TEXTURE_2D, texId);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(0);

  glEnable(GL_CULL_FACE);
}
