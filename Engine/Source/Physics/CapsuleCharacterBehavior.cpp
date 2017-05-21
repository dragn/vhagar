#include "Common.hpp"
#include "CapsuleCharacterBehavior.hpp"
#include "App/App.hpp"
#include "Actor/Actor.hpp"

using namespace vh;
using namespace physx;

vh::CapsuleCharacterBehavior::CapsuleCharacterBehavior(Actor* owner, float radius, float height)
    : ControlBehavior(owner)
    , mRadius(radius)
    , mHeight(height)
{

}

void vh::CapsuleCharacterBehavior::StartPlay()
{
    mPhysics = App::Get<Physics>();
    CHECK(mPhysics);

    // -- setup character controller
    PxCapsuleControllerDesc desc;
    desc.radius = mRadius;
    desc.height = 2 * mHeight;
    mMaterial = mPhysics->mPhysics->createMaterial(0.5f, 0.5f, 0.0);
    desc.material = mMaterial;
    mController = mPhysics->mControllerManager->createController(desc);
    if (mController == nullptr)
    {
        LOG(ERROR) << "Could not create controller!";
    }

    // -- get custom camera behavior
    mCamera = mOwner->GetBehaviorOfType<CameraBehavior>();
}

void vh::CapsuleCharacterBehavior::EndPlay()
{
    if (mController)
    {
        mController->release();
        mController = nullptr;
    }
    if (mMaterial)
    {
        mMaterial->release();
        mMaterial = nullptr;
    }
}

void vh::CapsuleCharacterBehavior::Tick(uint32_t delta)
{
    // -- update actor's position
    if (mController)
    {
        PxVec3 disp = PxVec3(0.0f, -1.0f, 0.0f); // apply gravity
        mController->move(disp, 0.001f, 1.0f / 60.f, mFilters);

        mOwner->SetPos(FromPhysX(mController->getPosition()));
    }
}

void vh::CapsuleCharacterBehavior::MoveForward(float value)
{
    if (mController)
    {
        PxVec3 disp = ToPhysX(mOwner->GetForward() * mWalkSpeed * value);
        mController->move(disp, 0.001f, 1.0f / 60.f, mFilters);
    }
}

void vh::CapsuleCharacterBehavior::MoveRight(float value)
{
    if (mController)
    {
        PxVec3 disp = ToPhysX(mOwner->GetRight() * mWalkSpeed * value);
        mController->move(disp, 0.001f, 1.0f / 60.f, mFilters);
    }
}

void vh::CapsuleCharacterBehavior::TurnRight(float value)
{
    // update character's yaw
    mOwner->AddYaw(- mTurnSpeed * value);
}

void vh::CapsuleCharacterBehavior::TurnUp(float value)
{
    if (mCamera)
    {
        mCamera->TurnUp(mTurnSpeed * value);
    }
}
