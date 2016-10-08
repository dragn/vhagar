#pragma once

#include <memory>
#include "SDL.h"

#ifdef WITH_GLOG
#include <glog/logging.h>
#else
#include <iostream>
#define LOG(X) std::cout << std::endl << #X << ": "
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
