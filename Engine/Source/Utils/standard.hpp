#pragma once

//////////////////////////////////////////////////////////////////////////
// Standard C++
#include <cstdint>
#include <new>
#include <list>
#include <string>
#include <exception>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

//////////////////////////////////////////////////////////////////////////
// Platform-specific
#if defined(CMAKE_PLATFORM_WINDOWS)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>
#include <TlHelp32.h>
#elif defined(CMAKE_PLATFORM_UNIX)
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <poll.h>
#endif

//////////////////////////////////////////////////////////////////////////
// OpenGL
#include <GL/glew.h>
#ifdef APPLE
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

//////////////////////////////////////////////////////////////////////////
// SDL
#define HAVE_M_PI
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

//////////////////////////////////////////////////////////////////////////
// GLM
#define GLM_FORCE_RADIANS

#ifdef _MSC_VER
// glm data alignment issue https://github.com/g-truc/glm/issues/283
#define GLM_FORCE_PURE
// disable secure function warnings
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
