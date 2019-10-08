#pragma once

#include "Actor/ActorBehavior.hpp"
#include "Utils/GLUtils.hpp"
#include "Renderer/Renderer.hpp"
#include "RenderableBehavior/RenderableBehavior.hpp"
#include "Renderable/SkyBox.hpp"

namespace vh
{

class SkyBoxBehavior : public RenderableBehavior<eRenderBlockType::SkyBox, SkyBox>
{
public:
    SkyBoxBehavior(std::shared_ptr<SkyBox> resource) : RenderableBehavior(resource) {};

    virtual void SetupPayload(typename SkyBox::Payload* payload);
};

} // namespace vh
