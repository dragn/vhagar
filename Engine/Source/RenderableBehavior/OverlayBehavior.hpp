#pragma once

#include "Renderable/Overlay.hpp"
#include "RenderableBehavior/RenderableBehavior.hpp"

namespace vh
{
    class OverlayBehavior : public RenderableBehavior<eRenderBlockType::Overlay, Overlay>
    {
    public:
        virtual void SetupPayload(Overlay::Payload* payload) override;
    };

} // namespace vh
