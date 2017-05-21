#pragma once

#include "PxPhysicsAPI.h"
#include "Math/Math.hpp"

namespace vh
{

physx::PxTransform ToPhysX(const M4& transform);
physx::PxVec3 ToPhysX(const V3& vec);
physx::PxQuat ToPhysX(const Quat& quat);

glm::quat FromPhysX(const physx::PxQuat& quat);
glm::vec3 FromPhysX(const physx::PxVec3& vec);

} // namespace vh
