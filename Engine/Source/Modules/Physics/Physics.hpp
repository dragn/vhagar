#pragma once

#include "Modules/standard.hpp"

namespace vh
{

class Physics : public vh::Module
{
    VH_MODULE(Physics);

    friend class PhysicsBehavior;
    friend class CapsuleCharacterBehavior;

public:
    Physics();

    virtual Ret TickInit(uint32_t delta) override;
    virtual Ret TickClose(uint32_t delta) override;

    virtual void StartFrame() override;
    virtual void EndFrame() override;

    physx::PxScene* GetScene()
    {
        return mScene;
    }

private:
    physx::PxFoundation* mFoundation = nullptr;
    physx::PxPhysics* mPhysics = nullptr;
    physx::PxTolerancesScale mScale;
    physx::PxScene* mScene = nullptr;
    physx::PxControllerManager* mControllerManager = nullptr;
    bool mSimCalled = false;
};

} // namespace vh
