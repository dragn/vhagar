#pragma once

#include "Modules/Renderer3D_GL/GLResources/GLOverlay.hpp"
#include "Modules/Renderer3D/RenderableBehavior/BOverlay.hpp"

namespace vh
{

    class BOverlay_GL : public BOverlay
    {
    public:
        BOverlay_GL() = default;
        BOverlay_GL(V2 const& pos, V2 const& size) : BOverlay(pos, size) {};

        virtual void StartPlay() override;
        virtual void EndPlay() override;
        virtual void Tick(uint32_t delta) override;

        virtual void Update() override;

    private:
        std::shared_ptr<GLOverlay> mGLOverlay;
    };

} // namespace vh