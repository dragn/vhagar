#include "Common.hpp"

#include "CameraActor.hpp"
#include <glm/gtx/transform.hpp>

vh::M4 vh::CameraActor::GetView() const
{
    return glm::lookAt(GetPos(), GetPos() + GetForward(), GetUp());
}
