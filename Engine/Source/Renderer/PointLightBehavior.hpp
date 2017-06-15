#pragma once

#include "Actor/ActorBehavior.hpp"
#include "Renderable/RenderableBehavior.hpp"
#include "Renderable/Lights.hpp"

namespace vh
{

class PointLightBehavior : public RenderableBehavior<eRenderBlockType::Light, PointLight>
{
public:
    PointLightBehavior(Actor* owner, float intensity);

    virtual void SetupPayload(PointLight::Payload* payload);

private:
    PointLight mLight;
    float mIntensity = 0.1f;
};

} // namespace vh
