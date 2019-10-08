#include "Vhagar_PCH.hpp"
#include "Core/App.hpp"
#include "Actor/Actor.hpp"
#include "Renderer/Renderer.hpp"
#include "PointLightBehavior.hpp"

namespace vh
{

PointLightBehavior::PointLightBehavior(float intensity)
    : mIntensity(intensity)
{
}

void PointLightBehavior::SetupPayload(PointLight::Payload* payload)
{
    payload->pos = GetPos();
    payload->intensity = mIntensity;
}

uint16_t PointLightBehavior::GetFlags()
{
    return eRenderBlockFlags::Active | eRenderBlockFlags::Interpolated;
}

} // namespace vh
