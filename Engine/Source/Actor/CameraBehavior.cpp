#include "Common.hpp"
#include "CameraBehavior.hpp"
#include "Actor/Actor.hpp"

using namespace vh;

vh::M4 vh::CameraBehavior::GetView()
{
    // Default implementation: use actor's position and orientation
    return glm::lookAt(GetPos(), GetPos() + GetForward(), GetOwner()->GetUp());
}

vh::V3 vh::CameraBehavior::GetPos()
{
    return GetOwner()->GetPos();
}

vh::V3 vh::CameraBehavior::GetForward()
{
    return GetOwner()->GetForward();
}
