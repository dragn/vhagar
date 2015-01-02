#pragma once

#include <glm/glm.hpp>

struct Camera {
  glm::vec3 position = glm::vec3(0, 0, 3);
  glm::vec3 direction = glm::vec3(0, 0, -1);
  glm::vec3 rotation = glm::vec3(1.0);
};
