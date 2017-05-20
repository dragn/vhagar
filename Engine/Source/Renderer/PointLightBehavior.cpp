#include "Common.hpp"
#include "App/App.hpp"
#include "Actor/Actor.hpp"
#include "Renderer.hpp"
#include "PointLightBehavior.hpp"

namespace vh
{

PointLightBehavior::PointLightBehavior(Actor* owner, float intensity)
    : ActorBehavior(owner)
    , mIntensity(intensity)
{
}

void PointLightBehavior::StartPlay()
{
    App::Get<Renderer>()->AddLight(this);
}

void PointLightBehavior::EndPlay()
{
    App::Get<Renderer>()->RemoveLight(this);
}

float PointLightBehavior::GetIntensity() const
{
    return mIntensity;
}

V3 PointLightBehavior::GetPos() const
{
    return GetOwner()->GetPos();
}

} // namespace vh
