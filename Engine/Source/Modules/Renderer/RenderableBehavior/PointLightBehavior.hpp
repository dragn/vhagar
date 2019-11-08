#pragma once

#include "Modules/Renderer/RenderableBehavior/RenderableBehavior.hpp"
#include "Modules/standard.hpp"

namespace vh
{

class PointLightBehavior : public RenderableBehavior
{
    VH_PROPERTY_RW(float, Intensity);

public:
    PointLightBehavior(float intensity) : mIntensity(intensity) {}
};

} // namespace vh
