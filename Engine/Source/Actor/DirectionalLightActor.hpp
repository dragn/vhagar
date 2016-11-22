#pragma once

#include "Actor.hpp"
#include "Renderer/Lights.hpp"

namespace vh
{

class DirectionalLightActor : public Actor, public DirectionalLight
{
public:
    virtual V3 GetDirection() const;
    virtual float GetIntensity() const;

    void SetIntensity(float intensity);

private:
    float mIntensity;
};

} // namespace vh