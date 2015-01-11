#include "GL3Renderer.hpp"
#include <glog/logging.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLUtils.hpp"
#include "../core/Application.hpp"

GLuint programID;

glm::mat4 Projection;

GLuint skyTexture;
GLuint skyMeshBuf;
GLuint skyMeshIdxBuf;
GLuint skyProgram = 0;
GLuint fbo;
GLuint width = Application::SCREEN_WIDTH;
GLuint height = Application::SCREEN_HEIGHT;

bool multisample = false;

ObjMesh skyMesh("models/skybox.obj");

GL3Renderer::GL3Renderer(SDL_Window *window) : window(window) {

  LOG(INFO) << "Initializing OpenGL renderer";
  LOG(INFO) << "OpenGL Version: " << glGetString(GL_VERSION);
  LOG(INFO) << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);

  programID = GLUtils::getShaderProgram("SimpleShader");

  Projection = glm::perspective(45.f, 4.0f / 3.0f, 0.1f, 100.f);

  // Enable backface culling
  glEnable(GL_CULL_FACE);
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_CUBE_MAP);

  int maxSamples;

  glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &maxSamples);
  multisample = (maxSamples > 1);
  LOG(INFO) << "Multi-sampling is: " << (multisample ? "enabled" : "disabled");

  if (multisample) {
    glEnable(GL_MULTISAMPLE);
    // setting up multisampling
    GLuint tex, depth_tex;
    GLuint num_samples = maxSamples >= 8 ? 8 : 1;
    glGenTextures( 1, &tex );
    glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, tex );
    glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_RGBA8, width, height, false );

    glGenTextures( 1, &depth_tex );
    glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, depth_tex );
    glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_DEPTH_COMPONENT, width, height, false );

    glGenFramebuffers( 1, &fbo );
    glBindFramebuffer( GL_FRAMEBUFFER, fbo );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0 );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depth_tex, 0 );
  }
}

void
GL3Renderer::prepare(const Scene &scene) {

  for (auto &info : buffers) {
    if (info.attribBuffer > 0) glDeleteBuffers(1, &info.attribBuffer);
    if (info.indexBuffer > 0) glDeleteBuffers(1, &info.indexBuffer);
  }

  models.clear();

  const Mesh *mesh;
  bufferInfo info;

  // Buffer all objects in the scene
  for (auto obj : *scene.objects()) {
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

  // Set up skybox
  //uvBuffer = GLUtils::bufferData(sizeof(uvData), uvData);
  if (scene.skyBox.pos_x.size() > 0) {
    skyTexture = GLUtils::loadCubeMapTexture(scene.skyBox);
    skyProgram = GLUtils::getShaderProgram("SkyDome");

    skyMeshBuf = GLUtils::bufferData(skyMesh._attribSize, skyMesh._vertexData.get());
    skyMeshIdxBuf = GLUtils::bufferElementArray(skyMesh._indexSize, skyMesh._indexData.get());
  }
}

void
GL3Renderer::render(const Scene &scene, const Object &camera) {

  M4 MVP;
  M4 View = glm::lookAt(camera.pos(), camera.pos() + camera.forward(), V3(0, 1, 0));

  if (multisample) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  }

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // render sky box
  if (skyProgram > 0) {
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
    glUseProgram(skyProgram);
    MVP = Projection * View * glm::scale(glm::translate(M4(1.0f), camera.pos()), V3(50.f, 50.f, 50.f));
    GLUtils::putUniformMat4(skyProgram, "MVP", MVP);

    glBindBuffer(GL_ARRAY_BUFFER, skyMeshBuf);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyMeshIdxBuf);
    glDrawElements(GL_TRIANGLES, skyMesh._indexSize, GL_UNSIGNED_INT, (void*) 0);

    glDisableVertexAttribArray(0);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
  }

  // render scene objects
  glUseProgram(programID);

  V3 lightPos = V3(0, 100, 0);
  GLUtils::putUniformVec3(programID, "LightPosition_worldspace", lightPos);

  bufferInfo info;
  GLuint size;

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

    glBindBuffer(GL_ARRAY_BUFFER, info.attribBuffer);

    GLfloat *offset = 0;
    size = info.attribSize;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, offset);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, offset + size);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, offset + 2 * size);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, offset + 3 * size);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, offset + 4 * size);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, info.indexBuffer);
    glDrawElements(GL_TRIANGLES, info.indexSize, GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
  }

  if (multisample) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);   // Make sure no FBO is set as the draw framebuffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo); // Make sure your multisampled FBO is the read framebuffer
    glDrawBuffer(GL_BACK);                       // Set the back buffer as the draw buffer
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  }

  SDL_GL_SwapWindow(window);
}
