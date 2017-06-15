#pragma once

#include <glm/glm.hpp>

namespace vh
{

struct RenderBufferHeader
{
    uint32_t time;
    uint32_t timestep;
    size_t size;
    glm::mat4 view;
    glm::mat4 projection;
};

namespace eRenderBlockFlags
{
const uint16_t Active = 1 << 0;
const uint16_t Interpolated = 1 << 1;
};

enum class eRenderBlockType : uint16_t
{
    Mesh = 1,
    RenderableLoad,
    RendarableRelease,
    Light,
    SkyBox,
    Overlay
};

namespace RenderBufferConstants
{
const size_t PAYLOAD_SIZE = 128;
const size_t MAX_RENDER_BLOCKS = 8192;
}

struct RenderBlock 
{
    uint16_t flags;
    eRenderBlockType type;
    uint8_t payload[RenderBufferConstants::PAYLOAD_SIZE];
};

struct RenderBuffer
{
    RenderBufferHeader header;
    RenderBlock blocks[RenderBufferConstants::MAX_RENDER_BLOCKS];
};

} // namespace vh
