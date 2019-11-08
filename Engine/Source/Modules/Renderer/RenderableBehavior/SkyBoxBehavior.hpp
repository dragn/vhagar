#pragma once

#include "Modules/Renderer/RenderableBehavior/RenderableBehavior.hpp"
#include "Modules/standard.hpp"

namespace vh
{

    class SkyBoxBehavior : public RenderableBehavior
    {
        VH_PROPERTY_RW(Utils::CubeMap, SkyBox);

    public:
        SkyBoxBehavior(Utils::CubeMap skybox) : mSkyBox(skybox) {};

        virtual void Tick(uint32_t delta) override
        {
            if (MRenderer3D* renderer = App::Get<MRenderer3D>())
            {
                renderer->TickRenderable<decltype(this)>(this);
            }
        }
    };

} // namespace vh
