#include <SDL2/SDL.h>
#include <glog/logging.h>
#include "core/Application.hpp"

int main(int argc, char ** argv) {
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  LOG(INFO) << "Starting application";

  LOG(INFO) << "SDL Initialization";
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    LOG(FATAL) << SDL_GetError();
    return 1;
  }

  Application app;

  try {
    app.run();
  } catch (...) {
    auto eptr = std::current_exception;
    if (eptr) LOG(ERROR) << "Exception occured";
  }

  LOG(INFO) << "Quitting...";
  SDL_Quit();
}
