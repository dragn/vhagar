#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glog/logging.h>
#include <GLES3/gl3.h>

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
