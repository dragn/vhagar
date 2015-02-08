#include "Renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLUtils.hpp"
#include "../RenderingObject/Mesh.hpp"
#include "../LightSource/LightSource.hpp"

using namespace Vhagar;

GLuint fbo;

bool multisample = false;

bool Renderer::init() {

  LOG(INFO) << "Initializing OpenGL renderer";
  LOG(INFO) << "OpenGL Version: " << glGetString(GL_VERSION);
  LOG(INFO) << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);

  projection = glm::perspective(45.f, 4.0f / 3.0f, 0.1f, 100.f);

  if (!GLEW_VERSION_3_0) {
    LOG(ERROR) << "Only OpenGL versions 3.0+ supported. Sorry.";

    isReady = false;
    return isReady;
  }

  // Enable backface culling
  glEnable(GL_CULL_FACE);
  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LEQUAL);
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
    glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_RGBA8, opts.screenWidth, opts.screenHeight, false );

    glGenTextures( 1, &depth_tex );
    glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, depth_tex );
    glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_DEPTH_COMPONENT, opts.screenWidth, opts.screenHeight, false );

    glGenFramebuffers( 1, &fbo );
    glBindFramebuffer( GL_FRAMEBUFFER, fbo );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0 );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depth_tex, 0 );
  }

  isReady = true;
  return isReady;
}

void
Renderer::addObject(RenderingObject &object) {
  if (!checkState()) return;

  objects.push_front(&object);
  object.beforeRender();
}

void
Renderer::removeObject(RenderingObject &object) {
  if (!checkState()) return;

  object.afterRender();
  objects.remove(&object);
}

void
Renderer::render() {
  if (!isReady) return;

  beforeRender();

  if (multisample) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
  }

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDisable(GL_BLEND);
  for (RenderingObject *obj : objects) {
    obj->render(projection, view);
  }
  if (lightSources.size()) {
    for (RenderingObject *obj : objects) {
      obj->render(projection, view, lightSources[0]);
    }
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  for (size_t i = 1; i < lightSources.size(); i++) {
    LightSource light = lightSources[i];
    for (RenderingObject *obj : objects) {
      obj->render(projection, view, light);
    }
  }

  if (multisample) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);   // Make sure no FBO is set as the draw framebuffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo); // Make sure your multisampled FBO is the read framebuffer
    glDrawBuffer(GL_BACK);                       // Set the back buffer as the draw buffer
    glBlitFramebuffer(0, 0, opts.screenWidth, opts.screenHeight, 0, 0, opts.screenWidth, opts.screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  }

  afterRender();
}

bool Renderer::checkState() {
  if (!isReady) {
    LOG(ERROR) << "Attempt to use uninitialized Renderer ignored!";
  }
  return isReady;
}
