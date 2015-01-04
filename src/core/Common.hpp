#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glog/logging.h>

typedef glm::vec3 V3;
typedef glm::mat4 M4;

template <typename T>
using uptr = std::unique_ptr<T>;
