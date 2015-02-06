#include <GL/glew.h>
#include "Common.hpp"
#include "Renderer/Renderer.hpp"
#include "RenderingObject/SkyBox.hpp"
#include "RenderingObject/Mesh.hpp"
#include "SDL.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace Vhagar;

SDL_Window *window;

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

class SDLRenderer : public Renderer {
  public:
    virtual void afterRender() {
      SDL_GL_SwapWindow(window);
    }
};

bool
handleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {

      case SDL_WINDOWEVENT:
        switch (event.window.event) {
          case SDL_WINDOWEVENT_CLOSE:
            event.type = SDL_QUIT;
            SDL_PushEvent(&event);
            break;
        }
        break;

      case SDL_QUIT:
        return false;
    }
  }
  return true;
}

int main(int argc, char **argv) {

#ifdef WITH_GLOG
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
#endif

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    LOG(FATAL) << SDL_GetError();
    std::exit(EXIT_FAILURE);
  }
  std::atexit(SDL_Quit);

  // Create Window
  window = SDL_CreateWindow("Renderer Test Program",
      0, 0, 800, 600, SDL_WINDOW_OPENGL);
  if (window == NULL) {
    LOG(FATAL) << "Unable to create window! " << SDL_GetError();
    std::exit(EXIT_FAILURE);
  }
  std::atexit([]{ SDL_DestroyWindow(window); });

  // Create GL context
  static SDL_GLContext glContext = SDL_GL_CreateContext(window);
  std::atexit([]{ SDL_GL_DeleteContext(glContext); });

  // Init GLEW
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    LOG(FATAL) << "Glew Error: " << glewGetErrorString(err);
    std::exit(EXIT_FAILURE);
  }

  // Init renderer
  SDLRenderer renderer;

  if (!renderer.init()) {
    std::exit(EXIT_FAILURE);
  }

  // Add some objects
  CubeMap map = {
    "images/space/pink_planet_neg_x.tga",
    "images/space/pink_planet_pos_x.tga",
    "images/space/pink_planet_pos_y.tga",
    "images/space/pink_planet_neg_y.tga",
    "images/space/pink_planet_pos_z.tga",
    "images/space/pink_planet_neg_z.tga"
  };

  SkyBox skyBox(map);

  ObjMesh ship("models/SimpleShip/Spaceship.obj");
  glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 4));
  glm::mat4 rot1 = glm::rotate(glm::mat4(1.0f), 2.3f, glm::vec3(0, 1, 0));
  glm::mat4 rot2 = glm::rotate(glm::mat4(1.0f), 0.3f, glm::vec3(1, 0, 0));
  ship.setModel(rot2 * rot1 * trans);

  float cameraAngle = 0;
  renderer.setView(glm::lookAt(glm::vec3(-2, 0, 15), glm::vec3(-2, 0, 0), glm::vec3(0, 1, 0)));
  renderer.addObject(&skyBox);
  renderer.addObject(&ship);

  glm::mat4 projection = glm::perspective(44.8f, 4.0f / 3.0f, 0.1f, 100.f);
  renderer.setProjection(projection);

  int ticks = 0, delta;
  // Main loop
  while (handleEvents()) {
    ticks = SDL_GetTicks();
    renderer.setView(glm::lookAt(glm::vec3(15 * cos(cameraAngle), 0, 15 * sin(cameraAngle)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
    renderer.render();

    delta = SDL_GetTicks() - ticks;
    cameraAngle += delta * 0.001;
    while (cameraAngle > 2 * M_PI) cameraAngle -= 2 * M_PI;
  }
}
