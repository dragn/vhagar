#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

M4 Camera::view() {
  // TODO calc matrix only when it's changed
  return glm::lookAt(pos(), pos() + forward(), V3(0, 1, 0));
}
