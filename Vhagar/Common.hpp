#pragma once

#define GLM_FORCE_RADIANS

#ifdef _MSC_VER
// glm data alignment issue https://github.com/g-truc/glm/issues/283
#define GLM_FORCE_PURE
// disable secure function warnins
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <memory>
#include "SDL.h"
#include <glm/glm.hpp>

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

typedef glm::vec4 V4;
typedef glm::vec3 V3;

typedef glm::mat4 M4;
typedef glm::mat3 M3;

class Rot {
  public:
    Rot() : yaw(0), pitch(0), roll(0) {};
    Rot(float yaw, float pitch, float roll): yaw(yaw), pitch(pitch), roll(roll) {};
    float yaw;
    float pitch;
    float roll;
};

template <typename T>
using uptr = std::unique_ptr<T>;

template <typename T>
using sptr = std::shared_ptr<T>;