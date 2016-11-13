#pragma once

#include "Actor.hpp"

#include "Renderer/Light.hpp"

namespace vh
{

class PointLight : public Actor, public Light
{
public:
    PointLight()
        : mIntensity(1.0f)
    {}

    PointLight(V3 pos, float intensity)
        : Actor(pos)
        , mIntensity(intensity)
    {}

    // Called when actor is created
    virtual void OnCreate();

    // Called when actor is destroyed
    virtual void OnDestroy();

    virtual float GetIntensity() const
    {
        return mIntensity;
    }

    virtual V3 GetPos() const
    {
        return Actor::GetPos();
    }

private:
    float mIntensity;
};

} // namespace vh
