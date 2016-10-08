#include "Vhagar.hpp"

using namespace vh;

Mesh::~Mesh() {
  LOG(INFO) << "Mesh destructor";
  if (indexSize > 0) free(indexData);
  if (attribSize > 0) free(attribData);
  if (texture != NULL) SDL_FreeSurface(texture);
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

  programID = Utils::getShaderProgram("SimpleShader");

  if (programID == 0) {
    LOG(ERROR) << "Object won't be rendered, because shader program invalid";
    // there was an error linking the program, do not proceed!
    return;
  }

  // specify sizes
  glInfo.attribBufferSize = attribSize;
  glInfo.indexBufferSize = indexSize;

  // buffer index data
  glInfo.indexBuffer = Utils::bufferElementArray(glInfo.indexBufferSize, indexData);

  // buffer attribute data
  glGenBuffers(1, &glInfo.attribBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, glInfo.attribBuffer);

  GLuint bytes = glInfo.attribBufferSize * sizeof(GLfloat);
  glBufferData(GL_ARRAY_BUFFER, attribCount * bytes, NULL, GL_STATIC_DRAW);

  for (size_t i = 0; i < attribCount; i++) {
    glBufferSubData(GL_ARRAY_BUFFER, i * bytes, bytes, attribData + i * attribSize);
  }

  // load texture
  if (texture != NULL) {
    glInfo.texture = Utils::bufferTexture2D(texture);
    LOG(INFO) << "Texture loaded with ID: " << glInfo.texture;
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
  if (glInfo.texture > 0) {
    LOG(INFO) << "Deleting previous texture: " << glInfo.texture;
    glDeleteTextures(1, &glInfo.texture);
  }
}

void
Mesh::render(glm::mat4 projection, glm::mat4 view, LightSource lightSource) {

  if (!isReadyToRender) return;

  MVP = projection * view * model;

  glUseProgram(programID);

  Utils::putUniformVec3(programID, "uLightPosition", lightSource.position);
  Utils::putUniformFloat(programID, "uLightIntensity", lightSource.intensity);

  Utils::putUniformMat4(programID, "uMVP", MVP);
  Utils::putUniformMat4(programID, "uM", model);
  Utils::putUniformMat4(programID, "uV", view);

  if (glInfo.texture) {
    glBindTexture(GL_TEXTURE_2D, glInfo.texture);
  }

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

void Mesh::setTexture(const std::string &filename) {
  if (glInfo.texture > 0) {
    LOG(INFO) << "Deleting previous texture: " << glInfo.texture;
    glDeleteTextures(1, &glInfo.texture);
    SDL_FreeSurface(texture);
  }

  LOG(INFO) << "Setting up texture " << filename;
  texture = Utils::loadImage(filename);
}
