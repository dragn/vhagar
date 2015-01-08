#include "Application.hpp"
#include "../render/GL3Renderer.hpp"
#include <glog/logging.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "../scenes/SpaceScene.hpp"

const Uint32 FPS_LIMIT = 60;

const Uint32 SCREEN_WIDTH = 1024;
const Uint32 SCREEN_HEIGHT = 768;

void
Application::run() {
  // Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
  window = SDL_CreateWindow(
      "GameEngine Demo", 0, 0,
      SCREEN_WIDTH, SCREEN_HEIGHT,
      SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

  windowID = SDL_GetWindowID(window);

  // Set relative mouse mod
  SDL_SetRelativeMouseMode(SDL_TRUE);

  //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

  // Create an OpenGL context associated with the window.
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);

  GL3Renderer renderer(window);
  uptr<Scene> scene(new SpaceScene());

  renderer.prepare(scene.get());

  playerController.camera.pos(V3(0, 0, 5));
  playerController.scene = scene.get();

  Object *ship = scene->getNamedObject("PlayerShip");
  if (ship != NULL) playerController.control(ship);
  else {
    LOG(FATAL) << "No PlayerShip found!";
  }

  Uint32 last = SDL_GetTicks();
  Uint32 next;
  Uint32 lastRender = last;
  Uint32 lastFPSprint = last;
  Uint32 frames = 0;
  Uint32 spf = 1000 / FPS_LIMIT;

  Scene *scenePtr = scene.get();
  Object *cameraPtr = &playerController.camera;

  // Main loop
  while (handleEvents()) {
    next = SDL_GetTicks();
    if (next != last) {
      tick(next - last);
      last = next;
    }
    if (next - lastRender > spf) {
      renderer.render(scenePtr, cameraPtr);
      int error = glGetError();
      if (error != GL_NO_ERROR) {
        LOG(ERROR) << "GL ERROR: " << error;
      }
      lastRender = next;
      frames++;
      if (lastRender - lastFPSprint > 1000) {
        LOG(INFO) << (lastRender - lastFPSprint) / frames << " ms/frame";
        lastFPSprint = lastRender;
        frames = 0;
      }
    }
  }

  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
}

bool
Application::handleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.window.windowID == windowID) {

      /* let player controller to handle event */
      playerController.handleEvent(&event);

      return handleEvent(&event);
    }
  }
  return true;
}

bool
Application::handleEvent(SDL_Event *event) {

  switch (event->type) {

    case SDL_WINDOWEVENT:
      switch (event->window.event) {
        case SDL_WINDOWEVENT_CLOSE:  
          event->type = SDL_QUIT;
          SDL_PushEvent(event);
          break;
      }
      break;

    case SDL_QUIT:
      return false;
  }

  return true;
}

void
Application::tick(Uint32 delta) {
  playerController.tick(delta);
}
