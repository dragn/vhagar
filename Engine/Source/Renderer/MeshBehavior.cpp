#include "Common.hpp"
#include "MeshBehavior.hpp"
#include "Renderer/Renderer.hpp"
#include "App/App.hpp"
#include "Resource/ResourceSystem.hpp"
#include "Utils/GLUtils.hpp"
#include "Actor/Actor.hpp"

vh::MeshBehavior::MeshBehavior(Actor* owner, const char* name)
    : RenderableBehavior(owner)
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
    payload->translate = GetOwner()->GetPos();
    payload->scale = GetOwner()->GetScale();
    payload->rotate = GetOwner()->GetQuat();
    payload->info = *Get()->GetBufferInfo();
    payload->dim = Get()->GetDim();
    payload->vertexCount = Get()->GetVertexCount();
    payload->owner = this;
}

bool vh::MeshBehavior::IsValid()
{
    GLuint size;
    GLuint* data;
    Get()->GetIndexData(size, data);
    return size > 0;
}
