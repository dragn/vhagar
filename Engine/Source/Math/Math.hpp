#pragma once

#define GLM_FORCE_RADIANS

#ifdef _MSC_VER
// glm data alignment issue https://github.com/g-truc/glm/issues/283
#define GLM_FORCE_PURE
// disable secure function warnins
#define _CRT_SECURE_NO_WARNINGS
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Math/Rot.hpp"

#include <ostream>

namespace vh
{

typedef glm::vec4 V4;
typedef glm::vec3 V3;
typedef glm::vec2 V2;

typedef glm::mat4 M4;
typedef glm::mat3 M3;

typedef glm::quat Quat;

} // namespace vh

namespace vh {
namespace Math {

M3 RotToM3(const Rot &rot);
M4 RotToM4(const Rot &rot);

float WrapAngle(float a, float min = - M_PI, float max = M_PI);

template<typename T>
T Clamp(T val, T min, T max)
{
    if (val < min) return min;
    else if (val > max) return max;
    else return val;
}

} // namespace Math
} // namespace vh

std::ostream& operator<<(std::ostream& os, const vh::V3& vec);
