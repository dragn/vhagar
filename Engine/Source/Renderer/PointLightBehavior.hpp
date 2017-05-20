#pragma once

#include "Actor/ActorBehavior.hpp"
#include "Lights.hpp"

namespace vh
{

class PointLightBehavior : public ActorBehavior, public PointLight
{
public:
    PointLightBehavior(Actor* owner, float intensity);

    // -- ActorBehavior interface
    virtual void StartPlay() override;
    virtual void EndPlay() override;

    // -- PointLight interface
    virtual float GetIntensity() const override;
    virtual V3 GetPos() const override;

private:
    float mIntensity = 0.1f;
};

} // namespace vh
