#pragma once

#include <memory>

#define HAVE_M_PI
#include "SDL.h"
#include "SDL_ttf.h"

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

#include "Math/Math.hpp"

template <typename T>
using uptr = std::unique_ptr<T>;

template <typename T>
using sptr = std::shared_ptr<T>;

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

} // namespace vh
