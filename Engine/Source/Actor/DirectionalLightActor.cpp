#include "Common.hpp"

#include "DirectionalLightActor.hpp"

vh::V3 vh::DirectionalLightActor::GetDirection() const
{
    return GetPos();
}

float vh::DirectionalLightActor::GetIntensity() const
{
    return mIntensity;
}

void vh::DirectionalLightActor::SetIntensity(float intensity)
{
    mIntensity = intensity;
}

