#include "Vhagar_PCH.hpp"
#include "ActorBehavior.hpp"
#include "Actor/Actor.hpp"

namespace vh
{

V3 ActorBehavior::GetPos()
{
    if (mParent)
    {
        return mParent->GetPos() + glm::toMat3(mParent->GetRot()) * mRelPos;
    }
    else
    {
        return mOwner->GetPos(); // root component position
    }
}

glm::quat ActorBehavior::GetRot()
{
    if (mParent)
    {
        return mParent->GetRot() * mRelRot;
    }
    else
    {
        return mOwner->GetQuat(); // root component rotation
    }
}

vh::V3 ActorBehavior::GetScale()
{
    if (mParent)
    {
        return mParent->GetScale() * mRelScale;
    }
    else
    {
        return mOwner->GetScale(); // root component scale
    }
}

} // namespace vh
