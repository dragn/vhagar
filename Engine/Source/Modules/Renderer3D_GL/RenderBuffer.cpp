#include "Modules/VhModules_PCH.hpp"
#include "RenderBuffer.hpp"

vh::RenderBlock* vh::RenderBuffer::AllocateNewBlock()
{
    if (header.size >= RenderBufferConstants::MAX_RENDER_BLOCKS)
    {
        LOG(WARNING) << "RenderBuffer overflow!";
        return nullptr;
    }

    return &blocks[header.size++];
}
