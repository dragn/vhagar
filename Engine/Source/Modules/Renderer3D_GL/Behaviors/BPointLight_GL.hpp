#pragma once

#include "Modules/Renderer/RenderableBehavior/BPointLight.hpp"
#include "Modules/Renderer3D_GL/RenderBuffer.hpp"

namespace vh
{
    /*
          //               \\
        BBBB PointLight_GL BBBB
          \\               //

        OpenGL implementation for point lights.
    */
    class BPointLight_GL : public BPointLight
    {
    public:
        BPointLight_GL() = default;
        BPointLight_GL(float intensity) : BPointLight(intensity) {}

        virtual void Tick(uint32_t delta) override;
    };

    namespace GLPointLight
    {
        struct Payload
        {
            glm::vec3 pos;
            float intensity;
        };
    }

    static_assert(sizeof(GLPointLight::Payload) <= RenderBufferConstants::PAYLOAD_SIZE, "invalid payload size");

} // namespace vh
