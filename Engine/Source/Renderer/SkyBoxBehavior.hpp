#pragma once

#include "Actor/ActorBehavior.hpp"
#include "Utils/GLUtils.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderable/RenderableBehavior.hpp"
#include "Renderable/SkyBox.hpp"

namespace vh
{

class SkyBoxBehavior : public RenderableBehavior<eRenderBlockType::SkyBox, SkyBox>
{
public:
    SkyBoxBehavior(Actor* owner, const Utils::CubeMap& cubeMap);

    virtual void SetupPayload(typename SkyBox::Payload* payload);

private:
    SkyBox mSkyBox;
};

} // namespace vh
