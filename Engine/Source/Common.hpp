#pragma once

#include <list>
#include <algorithm>
#include <memory>
#include <cstdint>
#include <regex>

#define HAVE_M_PI
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#ifdef WITH_GLOG
#include <glog/logging.h>
#else
#include <iostream>
#define LOG(X) std::cout << std::endl << #X << ": "
#define CHECK(X) assert(X)
#endif

#include <GL/glew.h>

#ifdef APPLE
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef WITH_PHYSX
#include "PxPhysicsAPI.h"
#include "Utils/PhysXUtils.hpp"
#endif // WITH_PHYSX

#include "Math/Math.hpp"

#define VH_STR(A) #A
#define VH_WSTR(A) L#A
#define VH_XSTR(A) VH_STR(A)
#define VH_XWSTR(A) VH_WSTR(A)
#define VH_CONCAT(A, B) A ## B

#define UNCOPYABLE(Class) \
  Class(const Class&) = delete; \
  Class& operator=(const Class&) = delete;

#define IF_KEYDOWN(event, code) if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == code)
#define IF_KEYDOWN_SYM(event, code) if (event->type == SDL_KEYDOWN && event->key.keysym.sym == code)

namespace vh
{

template<typename T, typename F>
void for_each(T t, F f)
{
    std::for_each(t.begin(), t.end(), f);
}

template <typename T>
void SafeDelete(T*& ptr)
{
    if (ptr) delete ptr;
    ptr = nullptr;
}

template <typename T>
void SafeDeleteArray(T*& ptr)
{
    if (ptr) delete [] ptr;
    ptr = nullptr;
}

} // namespace vh
