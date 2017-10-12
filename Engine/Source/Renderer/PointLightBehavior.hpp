#pragma once

#include "Actor/ActorBehavior.hpp"
#include "Renderable/RenderableBehavior.hpp"
#include "Renderable/Lights.hpp"

namespace vh
{

class PointLightBehavior : public RenderableBehavior<eRenderBlockType::Light, PointLight>
{
public:
    PointLightBehavior(float intensity);

    virtual void SetupPayload(PointLight::Payload* payload);

    virtual uint16_t GetFlags() override;

private:
    float mIntensity = 0.1f;
};

} // namespace vh
