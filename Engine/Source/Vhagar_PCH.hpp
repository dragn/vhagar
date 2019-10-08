#pragma once

#include <list>
#include <algorithm>
#include <memory>
#include <cstdint>
#include <regex>
#include <string>

#define HAVE_M_PI
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include <GL/glew.h>

#ifdef APPLE
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef WITH_PHYSX
#include "PxPhysicsAPI.h"
#endif // WITH_PHYSX
