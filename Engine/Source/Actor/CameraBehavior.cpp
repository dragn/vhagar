#include "Common.hpp"
#include "CameraBehavior.hpp"
#include "Actor/Actor.hpp"

using namespace vh;

vh::M4 vh::CameraBehavior::GetView()
{
    // Default implementation: use actor's position and orientation
    return GetOwner()->GetView();
}
