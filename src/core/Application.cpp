#include "Application.hpp"
#include "../render/GL3Renderer.hpp"
#include <glog/logging.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>

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

  Uint32 last = SDL_GetTicks();
  Uint32 next;
  Uint32 lastRender = SDL_GetTicks();
  while (handleEvents()) {
    next = SDL_GetTicks();
    tick(next - last);
    last = next;
    if (lastRender - SDL_GetTicks() > 40) {
      rndr->render(&scene);
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
      return handleEvent(&event);
    }
  }
  return true;
}

bool
Application::handleEvent(SDL_Event *event) {

  switch (event->type) {

    case SDL_KEYDOWN:
    case SDL_KEYUP:
      handleKey(event->type, &event->key);
      break;

    case SDL_MOUSEMOTION:
      handleMouse(event->motion.xrel, event->motion.yrel);
      break;

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

glm::vec3 camVel = glm::vec3(0,0,0);
float camSpeed = 5;
float camTurnSpeed = 0.2;

void
Application::handleKey(Uint32 type, SDL_KeyboardEvent *event) {
  SDL_Keycode k = event->keysym.sym;
  glm::vec3 dir = scene.camera.direction;
  switch (type) {
    case SDL_KEYDOWN:
      if (k == SDLK_w) {
        camVel = camSpeed * dir;
      } else if (k == SDLK_s) {
        camVel = - camSpeed * dir;
      } else if (k == SDLK_a) {
        camVel = - camSpeed * glm::cross(dir, glm::vec3(0, 1, 0));
      } else if (k == SDLK_d) {
        camVel = camSpeed * glm::cross(dir, glm::vec3(0, 1, 0));
      }
      break;
    case SDL_KEYUP:
      if (k == SDLK_w) {
        camVel = glm::vec3(0,0,0);
      } else if (k == SDLK_s) {
        camVel = glm::vec3(0,0,0);
      } else if (k == SDLK_a) {
        camVel = glm::vec3(0,0,0);
      } else if (k == SDLK_d) {
        camVel = glm::vec3(0,0,0);
      }
      break;
  }
}

void
Application::handleMouse(Sint32 xrel, Sint32 yrel) {
  scene.camera.direction = glm::rotateY(scene.camera.direction, - xrel * camTurnSpeed);
}

void
Application::tick(Uint32 time) {
  float t = time / 1000.0f;
  scene.camera.position += camVel * t;
}
