#include "Modules/VhModules_PCH.hpp"
#include "BOverlay.hpp"

vh::BOverlay::~BOverlay()
{
    if (mSurf)
    {
        SDL_FreeSurface(mSurf);
    }
}

/*
void vh::OverlayBehavior::SetupPayload(Overlay::Payload* payload)
{
    payload->progId = mResource->mShaderId;
    payload->texId = mResource->mTexId;
    payload->vertexBuffer = mResource->mVertexBuffer;
    payload->bounds = mResource->mBounds;
}
*/

void vh::BOverlay::InitSurface()
{
    if (mSurf)
    {
        SDL_FreeSurface(mSurf);
    }

    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;

    mSurf = SDL_CreateRGBSurface(0, mSize.x, mSize.y, 32, rmask, gmask, bmask, amask);
}
