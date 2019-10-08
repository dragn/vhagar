#pragma once

#include "Modules/Renderer/Renderable/Overlay.hpp"
#include "Modules/Renderer/RenderableBehavior/RenderableBehavior.hpp"

namespace vh
{
    class OverlayBehavior : public RenderableBehavior<eRenderBlockType::Overlay, Overlay>
    {
    public:
        virtual void SetupPayload(Overlay::Payload* payload) override;
    };

} // namespace vh
