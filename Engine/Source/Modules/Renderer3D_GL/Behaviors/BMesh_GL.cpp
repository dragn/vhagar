#include "Modules/VhModules_PCH.hpp"
#include "BMesh_GL.hpp"

#include "Modules/ResourceSystem/MResourceSystem.hpp"
#include "Modules/Renderer3D_GL/MRenderer3D_GL.hpp"

void vh::BMesh_GL::StartPlay()
{
    mGLMesh = App::Get<MResourceSystem>()->GetResource<GLMesh>(GetPath().c_str());
    CHECK(mGLMesh);

    App::Get<MRenderer3D_GL>()->LoadRes(mGLMesh);
}

void vh::BMesh_GL::EndPlay()
{
    App::Get<MRenderer3D_GL>()->UnloadRes(mGLMesh);
    mGLMesh.reset();
}

void vh::BMesh_GL::Tick(uint32_t delta)
{
    if (!mGLMesh->IsLoaded())
    {
        return;
    }

    RenderBlock* block = App::Get<MRenderer3D_GL>()->GetWriteBuffer().AllocateNewBlock();
    CHECK(block);

    block->type = eRenderBlockType::Mesh;
    block->flags = eRenderBlockFlags::Active | eRenderBlockFlags::Interpolated;

    GLMesh::Payload* payload = reinterpret_cast<GLMesh::Payload*>(block->payload);
    payload->progId = mGLMesh->GetShaderId();
    payload->translate = GetPos();
    payload->scale = GetScale();
    payload->rotate = GetRot();
    payload->info = mGLMesh->GetBufferInfo();
    payload->dim = mGLMesh->GetDim();
    payload->vertexCount = mGLMesh->GetVertexCount();
    payload->owner = this;
    payload->ignoreDepth = GetIgnoreDepth();
}
