#include "Modules/VhModules_PCH.hpp"
#include "OverlayBehavior.hpp"

void vh::OverlayBehavior::SetupPayload(Overlay::Payload* payload)
{
    payload->progId = mResource->mShaderId;
    payload->texId = mResource->mTexId;
    payload->vertexBuffer = mResource->mVertexBuffer;
    payload->bounds = mResource->mBounds;
}
