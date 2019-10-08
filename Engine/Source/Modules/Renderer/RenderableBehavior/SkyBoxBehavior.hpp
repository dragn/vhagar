#pragma once

#include "Core/Actor/ActorBehavior.hpp"
#include "Utils/GLUtils.hpp"
#include "Modules/Renderer/Renderer.hpp"
#include "Modules/Renderer/RenderableBehavior/RenderableBehavior.hpp"
#include "Modules/Renderer/Renderable/SkyBox.hpp"

namespace vh
{

class SkyBoxBehavior : public RenderableBehavior<eRenderBlockType::SkyBox, SkyBox>
{
public:
    SkyBoxBehavior(std::shared_ptr<SkyBox> resource) : RenderableBehavior(resource) {};

    virtual void SetupPayload(typename SkyBox::Payload* payload);
};

} // namespace vh
