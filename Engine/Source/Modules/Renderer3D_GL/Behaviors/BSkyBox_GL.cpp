#include "Modules/VhModules_PCH.hpp"
#include "BSkyBox_GL.hpp"

#include "Modules/Renderer3D_GL/MRenderer3D_GL.hpp"

void vh::BSkyBox_GL::StartPlay()
{
    mGLSkyBox = std::make_shared<GLSkyBox>(GetRes()->GetCubeMap());
    App::Get<MRenderer3D_GL>()->LoadRes(mGLSkyBox);
}

void vh::BSkyBox_GL::EndPlay()
{
    App::Get<MRenderer3D_GL>()->UnloadRes(mGLSkyBox);
    mGLSkyBox.reset();
}

void vh::BSkyBox_GL::Tick(uint32_t delta)
{
    if (!mGLSkyBox->IsLoaded())
    {
        return;
    }

    RenderBuffer& buffer = App::Get<MRenderer3D_GL>()->GetWriteBuffer();
    RenderBlock* block = buffer.AllocateNewBlock();
    CHECK(block);

    block->type = eRenderBlockType::SkyBox;
    block->flags = eRenderBlockFlags::Active;

    GLSkyBox::Payload& payload = *(reinterpret_cast<GLSkyBox::Payload*>(block->payload));
    mGLSkyBox->SetupPayload(payload);
}
