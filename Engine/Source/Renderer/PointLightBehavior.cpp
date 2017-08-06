#include "Common.hpp"
#include "App/App.hpp"
#include "Actor/Actor.hpp"
#include "Renderer.hpp"
#include "PointLightBehavior.hpp"

namespace vh
{

PointLightBehavior::PointLightBehavior(float intensity)
    : mIntensity(intensity)
{
    Set(&mLight);
}

void PointLightBehavior::SetupPayload(PointLight::Payload* payload)
{
    payload->pos = GetOwner()->GetPos();
    payload->intensity = mIntensity;
}

} // namespace vh
