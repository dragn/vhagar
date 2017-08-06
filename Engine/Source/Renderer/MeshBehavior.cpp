#include "Common.hpp"
#include "MeshBehavior.hpp"
#include "Renderer/Renderer.hpp"
#include "App/App.hpp"
#include "Resource/ResourceSystem.hpp"
#include "Utils/GLUtils.hpp"
#include "Actor/Actor.hpp"

vh::MeshBehavior::MeshBehavior(const char* name)
    : mIgnoreDepth(false)
{
    ResourceSystem* resource = App::Get<ResourceSystem>();
    CHECK(resource);

    std::shared_ptr<Mesh> mesh = resource->GetMesh(name);
    if (!mesh)
    {
        LOG(WARNING) << "Could not load mesh by name " << name;
    }
    else
    {
        Set(mesh.get());
    }
}

void vh::MeshBehavior::SetupPayload(Mesh::Payload* payload)
{
    payload->progId = Get()->GetShaderId();
    payload->translate = GetPos();
    payload->scale = GetScale();
    payload->rotate = GetRot();
    payload->info = *Get()->GetBufferInfo();
    payload->dim = Get()->GetDim();
    payload->vertexCount = Get()->GetVertexCount();
    payload->owner = this;
    payload->ignoreDepth = mIgnoreDepth;
}

bool vh::MeshBehavior::IsValid()
{
    GLuint size;
    GLuint* data;
    Get()->GetIndexData(size, data);
    return size > 0;
}
