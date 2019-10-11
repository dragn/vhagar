#pragma once

#include "Modules/Renderer/MRenderer3D.hpp"
#include "Modules/Renderer/RenderableBehavior/RenderableBehavior.hpp"
#include "Modules/Renderer/Renderable/SkyBox.hpp"
#include "Modules/standard.hpp"

namespace vh
{

class SkyBoxBehavior : public RenderableBehavior<eRenderBlockType::SkyBox, SkyBox>
{
public:
    SkyBoxBehavior(std::shared_ptr<SkyBox> resource) : RenderableBehavior(resource) {};

    virtual void SetupPayload(typename SkyBox::Payload* payload);
};

} // namespace vh
