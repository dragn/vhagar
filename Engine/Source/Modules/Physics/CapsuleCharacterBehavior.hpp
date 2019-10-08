#pragma once

#include "Modules/World/ControlBehavior.hpp"
#include "Modules/World/CameraBehavior.hpp"
#include "Physics.hpp"

namespace vh
{

/*
    Control behavior that uses PhysX character controller for
    movement inside a scene.
    Uses Capsule geometry for collision response.
*/
class CapsuleCharacterBehavior : public ControlBehavior, public physx::PxUserControllerHitReport
{
public:
    CapsuleCharacterBehavior(float radius, float height);

    // -- ActorBehavior interface
    virtual void StartPlay() override;
    virtual void EndPlay() override;
    virtual void Tick(uint32_t delta) override;

    // -- ControlBehavior interface
    virtual void MoveForward(float value) override;
    virtual void MoveRight(float value) override;
    virtual void TurnRight(float value) override;
    virtual void TurnUp(float value) override;

    // -- PxUserControllerHitReport interface
    virtual void onShapeHit(const physx::PxControllerShapeHit& hit) override {};
    virtual void onControllerHit(const physx::PxControllersHit &hit) override {};
    virtual void onObstacleHit(const physx::PxControllerObstacleHit &hit) override {};

private:
    float mRadius;
    float mHeight;

    Physics* mPhysics = nullptr;
    physx::PxController* mController = nullptr;
    physx::PxMaterial* mMaterial = nullptr;
    CameraBehavior* mCamera = nullptr;
    float mTurnSpeed = 0.001f;
    float mWalkSpeed = 0.1f;
    physx::PxControllerFilters mFilters;
    float mCameraPitch = 0.0f;
};

} // namespace vh
