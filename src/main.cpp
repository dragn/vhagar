#include "core/Common.hpp"
#include "core/Application.hpp"

int main(int argc, char ** argv) {

#ifdef WITH_GLOG
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
#endif

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
