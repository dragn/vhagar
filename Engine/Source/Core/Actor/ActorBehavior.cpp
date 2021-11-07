#include "Core/VhCore_PCH.hpp"

#include "ActorBehavior.hpp"
#include "Actor.hpp"

namespace vh
{
    std::unordered_map<std::type_index, std::function<std::unique_ptr<vh::ActorBehavior>()>> sOverrides;

    std::unordered_map<std::type_index, std::function<std::unique_ptr<vh::ActorBehavior>()>>& ActorBehavior::GetOverrides()
    {
        return sOverrides;
    }

    V3 ActorBehavior::GetPos() const
    {
        if (mParent)
        {
            return mParent->GetPos() + glm::toMat3(mParent->GetRot()) * mRelPos;
        }
        else
        {
            return LockOwner()->GetPos(); // root component position
        }
    }

    glm::quat ActorBehavior::GetRot() const
    {
        if (mParent)
        {
            return mParent->GetRot() * mRelRot;
        }
        else
        {
            return LockOwner()->GetQuat(); // root component rotation
        }
    }

    vh::V3 ActorBehavior::GetScale() const
    {
        if (mParent)
        {
            return mParent->GetScale() * mRelScale;
        }
        else
        {
            return LockOwner()->GetScale(); // root component scale
        }
    }

} // namespace vh
