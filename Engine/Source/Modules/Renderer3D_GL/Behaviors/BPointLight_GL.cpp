#include "Modules/VhModules_PCH.hpp"
#include "BPointLight_GL.hpp"

#include "Modules/Renderer3D_GL/MRenderer3D_GL.hpp"

void vh::BPointLight_GL::Tick(uint32_t delta)
{
    RenderBlock* block = App::Get<MRenderer3D_GL>()->GetWriteBuffer().AllocateNewBlock();
    CHECK(block);

    block->type = eRenderBlockType::Light;
    block->flags = eRenderBlockFlags::Active;
    GLPointLight::Payload& payload = *reinterpret_cast<GLPointLight::Payload*>(block->payload);
    payload.pos = GetPos();
    payload.intensity = GetIntensity();
}
