#pragma once

#include "Actor/ActorBehavior.hpp"
#include "Utils/GLUtils.hpp"
#include "Renderer/Renderer.hpp"

namespace vh
{

class SkyBoxBehavior : public ActorBehavior
{
public:
    SkyBoxBehavior(Actor* owner, const Utils::CubeMap& cubeMap);

    // -- ActorBehavior interface
    virtual void StartPlay() override;
    virtual void EndPlay() override;
    virtual void Tick(uint32_t delta) override;

private:
    GLBufferInfo mGLInfo;
    GLuint mTexture;
    GLuint mProgramID;
    Utils::CubeMap mCubeMap;
    Renderer* mRenderer;
};

} // namespace vh
