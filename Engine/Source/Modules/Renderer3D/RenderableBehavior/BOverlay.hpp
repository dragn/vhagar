#pragma once

#include "Modules/Renderer3D/RenderableBehavior/RenderableBehavior.hpp"

namespace vh
{
    class BOverlay : public ActorBehavior
    {
        VH_PROPERTY_RO(V2, Pos);
        VH_PROPERTY_RO(V2, Size);
        VH_PROPERTY_RO(SDL_Surface*, Surf);

    public:
        BOverlay() : mSurf(nullptr) {}
        BOverlay(V2 const& pos, V2 const& size)
            : mPos(pos)
            , mSize(size)
            , mSurf(nullptr)
        {
            InitSurface();
        }
        void operator=(BOverlay const& other)
        {
            mPos = other.mPos;
            mSize = other.mSize;
            InitSurface();
        }
        virtual ~BOverlay();

        virtual void Update() {};

    private:
        void InitSurface();
    };

} // namespace vh
