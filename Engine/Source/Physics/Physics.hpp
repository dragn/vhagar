#pragma once

#include "App/Component.hpp"
#include "PxPhysicsAPI.h"

namespace vh
{

class Physics : public vh::Component
{
    COMPONENT_NAME(Physics);

public:
    Physics();

    virtual void TickInit(uint32_t delta) override;
    virtual void TickRun(uint32_t delta) override;
    virtual void TickClose(uint32_t delta) override;

private:
    physx::PxFoundation* mFoundation;
    physx::PxPhysics* mPhysics;
    physx::PxTolerancesScale mScale;
    physx::PxScene* mScene;
};

} // namespace vh
