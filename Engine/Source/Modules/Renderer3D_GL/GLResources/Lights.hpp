#pragma once

#include "Modules/Renderer3D_GL/RenderBuffer.hpp"
#include "Modules/Renderer3D_GL/GLResources/GLResource.hpp"

namespace vh
{
    class DirectionalLight
    {
    public:
        virtual V3 GetDirection() const = 0;
        virtual float GetIntensity() const = 0;
    };

    class AmbientLight
    {
    public:
        virtual float GetIntensity() const = 0;
    };

} // namespace vh
