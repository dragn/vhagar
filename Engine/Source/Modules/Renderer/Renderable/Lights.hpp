#pragma once

#include "Modules/Renderer/RenderBuffer.hpp"
#include "Modules/Renderer/Renderable/Renderable.hpp"

namespace vh {

class PointLight : public Renderable
{
public:
    struct Payload
    {
        glm::vec3 pos;
        float intensity;
    };

    virtual bool DoLoad() { return true; }
    virtual bool DoUnload() { return true; }
};

static_assert(sizeof(PointLight) <= RenderBufferConstants::PAYLOAD_SIZE, "invalid payload size");

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
