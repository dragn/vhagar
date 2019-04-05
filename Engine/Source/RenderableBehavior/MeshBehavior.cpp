#include "Vhagar_PCH.hpp"
#include "MeshBehavior.hpp"
#include "App/App.hpp"
#include "Utils/GLUtils.hpp"
#include "Actor/Actor.hpp"

vh::MeshBehavior::MeshBehavior(const char* name)
    : RenderableBehavior(name), mIgnoreDepth(false)
{
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
