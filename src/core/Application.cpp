#include "Application.hpp"
#include "../render/GL3Renderer.hpp"
#include <glog/logging.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "../scenes/SpaceScene.hpp"

void
Application::run() {
  // Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
  window = SDL_CreateWindow(
      "GameEngine Demo", 0, 0, 800, 600, 
      SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

  windowID = SDL_GetWindowID(window);

  // Set relative mouse mod
  SDL_SetRelativeMouseMode(SDL_TRUE);

  // Create an OpenGL context associated with the window.
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);

  GL3Renderer *rndr = new GL3Renderer(window);

  scene.reset(new SpaceScene());
  rndr->prepare(scene.get());

  scene->camera.pos(V3(0, 0, 5));
  playerController.control(&scene->camera);

  Uint32 last = SDL_GetTicks();
  Uint32 next;
  Uint32 lastRender = SDL_GetTicks();
  while (handleEvents()) {
    next = SDL_GetTicks();
    tick(next - last);
    last = next;
    if (lastRender - SDL_GetTicks() > 40) {
      rndr->render(scene.get());
      lastRender = SDL_GetTicks();
    }
  }

  // Once finished with OpenGL functions, the SDL_GLContext can be deleted.
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
