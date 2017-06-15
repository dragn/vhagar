#include "Common.hpp"
#include "MeshBehavior.hpp"
#include "Renderer/Renderer.hpp"
#include "App/App.hpp"
#include "Resource/ResourceSystem.hpp"
#include "Utils/GLUtils.hpp"
#include "Actor/Actor.hpp"

namespace
{
void reportGLError()
{
    int error = glGetError();
    switch (error)
    {
    case GL_NO_ERROR:
        return;
    case GL_INVALID_OPERATION:
        LOG(ERROR) << "GL ERROR: INVALID_OPERATION";
        return;
    case GL_INVALID_VALUE:
        LOG(ERROR) << "GL ERROR: INVALID_VALUE";
        return;
    default:
        LOG(ERROR) << "GL ERROR: " << error;
    }
}
}

vh::MeshBehavior::MeshBehavior(Actor* owner, const char* name)
    : RenderableBehavior(owner)
{
    ResourceSystem* resource = App::Get<ResourceSystem>();
    CHECK(resource);

    mMesh = resource->GetMesh(name);
    if (!mMesh)
    {
        LOG(WARNING) << "Could not load mesh by name " << name;
    }
    else
    {
        Set(mMesh.get());
    }
}

void vh::MeshBehavior::SetupPayload(Mesh::Payload* payload)
{
    payload->progId = mMesh->GetShaderId();
    payload->translate = GetOwner()->GetPos();
    payload->scale = GetOwner()->GetScale();
    payload->rotate = GetOwner()->GetQuat();
    payload->info = *mMesh->GetBufferInfo();
    payload->dim = mMesh->GetDim();
    payload->vertexCount = mMesh->GetVertexCount();
    payload->owner = this;
}

bool vh::MeshBehavior::IsValid()
{
    GLuint size;
    GLuint* data;
    mMesh->GetIndexData(size, data);
    return size > 0;
}
