#include "Common.hpp"
#include "PhysicsBehavior.hpp"

vh::PhysicsBehavior::~PhysicsBehavior()
{
    SafeDelete(mCollisionGeometry);
}
