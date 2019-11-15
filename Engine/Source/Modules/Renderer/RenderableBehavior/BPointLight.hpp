#pragma once

#include "Modules/Renderer/RenderableBehavior/RenderableBehavior.hpp"
#include "Modules/standard.hpp"

namespace vh
{
    /*
          //            \\
        BBBB PointLight BBBB
          \\            //

        Adds a point light source to an actor.
    */

    class BPointLight : public ActorBehavior
    {
        VH_PROPERTY_RW(float, Intensity);

    public:
        BPointLight() = default;
        BPointLight(float intensity) : mIntensity(intensity) {}

        void operator=(BPointLight const& other)
        {
            mIntensity = other.mIntensity;
        }
    };

} // namespace vh
