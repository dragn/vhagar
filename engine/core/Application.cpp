#include "Application.hpp"
#include "../render/GL3Renderer.hpp"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "../scenes/SpaceScene.hpp"

const Uint32 FPS_LIMIT = 60;

void reportGLError(int error) {
  switch (error) {
    case GL_NO_ERROR:
      return;
    case GL_INVALID_OPERATION:
      LOG(ERROR) << "GL ERROR: INVALID_OPERATION";
      return;
    case GL_INVALID_VALUE:
      LOG(ERROR) << "GL ERROR: INVALID_VALUE";
      return;
    default:
      LOG(ERROR) << "GL ERROR: " << error;
  }
}

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

  // Create an OpenGL context associated with the window.
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    LOG(FATAL) << "Glew Error: " << glewGetErrorString(err);
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    return;
  }

  GL3Renderer renderer(window);
  SpaceScene scene;

  renderer.prepare(scene);

  playerController.camera.pos(V3(0, 0, 5));
  playerController.scene = &scene;

  Object *ship = scene.getNamedObject("PlayerShip");
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
  Uint32 error;

  // Main loop
  while (handleEvents()) {
    next = SDL_GetTicks();
    if (next != last) {
      tick(next - last);
      last = next;
    }
    if (next - lastRender > spf) {
      renderer.render(scene, playerController.camera);
      error = glGetError();
      if (error) reportGLError(error);
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
