#include "Utils/VhUtils_PCH.hpp"

#include "PhysXUtils.hpp"

physx::PxTransform vh::ToPhysX(const M4& transform)
{
    physx::PxTransform t;
    t.p = { transform[3].x, transform[3].y, transform[3].z };
    glm::quat quat = glm::quat_cast(transform);
    t.q = { quat.x, quat.y, quat.z, quat.w };
    return t;
}

physx::PxVec3 vh::ToPhysX(const V3& vec)
{
    return physx::PxVec3(vec.x, vec.y, vec.z);
}

physx::PxQuat vh::ToPhysX(const Quat& quat)
{
    return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
}

glm::quat vh::FromPhysX(const physx::PxQuat& quat)
{
    // look out! glm quat's constructor accepts coordinates in this order: w, x, y, z
    return glm::quat(quat.w, quat.x, quat.y, quat.z);
}

glm::vec3 vh::FromPhysX(const physx::PxExtendedVec3& vec)
{
    return V3(vec.x, vec.y, vec.z);
}

glm::vec3 vh::FromPhysX(const physx::PxVec3& vec)
{
    return V3(vec.x, vec.y, vec.z);
}
