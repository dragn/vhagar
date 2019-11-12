#pragma once

#include "Modules/Renderer/RenderableBehavior/RenderableBehavior.hpp"

namespace vh
{
    class OverlayBehavior : public RenderableBehavior
    {
        VH_PROPERTY_RW(V2, Pos);
        VH_PROPERTY_RW(SDL_Surface*, Texture);

    public:
        OverlayBehavior(V2 pos = V2(0.0f, 0.0f))
            : mPos(pos)
        {}
    };

} // namespace vh
