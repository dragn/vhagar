#include "Modules/VhModules_PCH.hpp"
#include "BOverlay_GL.hpp"

#include "Modules/Renderer3D_GL/MRenderer3D_GL.hpp"

void vh::BOverlay_GL::StartPlay()
{
    mGLOverlay = std::make_shared<GLOverlay>(GetPos(), GetSize());
    App::Get<MRenderer3D_GL>()->LoadRes(mGLOverlay);
}

void vh::BOverlay_GL::EndPlay()
{
    App::Get<MRenderer3D_GL>()->UnloadRes(mGLOverlay);
    mGLOverlay.reset();
}

void vh::BOverlay_GL::Tick(uint32_t delta)
{
    CHECK(mGLOverlay);
    if (!mGLOverlay->IsLoaded())
    {
        return;
    }

    RenderBlock* block = App::Get<MRenderer3D_GL>()->GetWriteBuffer().AllocateNewBlock();
    CHECK(block);

    block->type = eRenderBlockType::Overlay;
    block->flags = eRenderBlockFlags::Active;
    GLOverlay::Payload& payload = *reinterpret_cast<GLOverlay::Payload*>(block->payload);
    mGLOverlay->SetupPayload(payload);
}

void vh::BOverlay_GL::Update()
{
    mGLOverlay->BlitTexture(GetSurf());
}

