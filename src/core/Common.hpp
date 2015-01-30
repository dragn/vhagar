#pragma once

#define GLM_FORCE_RADIANS
#define GL_GLEXT_PROTOTYPES

#include <memory>
#include "SDL.h"
#include <glm/glm.hpp>

#ifdef WITH_GLOG
#include <glog/logging.h>
#else
#include <iostream>
#define LOG(X) std::cout
#endif

#include <GL/gl.h>
#include <GL/glext.h>

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
