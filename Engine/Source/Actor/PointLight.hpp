#pragma once

#include "Actor.hpp"

#include "Renderer/Light.hpp"

namespace vh
{

class PointLight : public Actor
{
public:
    PointLight(V3 pos, float intensity) :
        Actor(pos),
        mIntensity(intensity)
    {}

    // Called when actor is created
    virtual void OnCreate();

    // Called when actor is destroyed
    virtual void OnDestroy();

private:
    float mIntensity;

    size_t mLightId;

};

} // namespace vh