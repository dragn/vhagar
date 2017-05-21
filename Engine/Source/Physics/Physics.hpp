#pragma once

#include "App/Component.hpp"
#include "PxPhysicsAPI.h"

namespace vh
{

class Physics : public vh::Component
{
    COMPONENT_NAME(Physics);

    friend class PhysicsBehavior;

public:
    Physics();

    virtual void TickInit(uint32_t delta) override;
    virtual void TickRun(uint32_t delta) override;
    virtual void TickClose(uint32_t delta) override;

    virtual void StartFrame() override;
    virtual void EndFrame() override;

private:
    physx::PxFoundation* mFoundation = nullptr;
    physx::PxPhysics* mPhysics = nullptr;
    physx::PxTolerancesScale mScale;
    physx::PxScene* mScene = nullptr;
};

} // namespace vh
