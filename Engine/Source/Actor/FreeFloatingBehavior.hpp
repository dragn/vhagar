#pragma once

#include "Math/Math.hpp"
#include "ControlBehavior.hpp"

namespace vh
{

class FreeFloatingBehavior : public ControlBehavior
{
public:
    FreeFloatingBehavior(Actor* owner)
        : ControlBehavior(owner)
        , mMoveSpeed(5)
        , mTurnSpeed(0.001f)
    {}

    virtual void MoveForward(float value) override
    {
        mOwner->AddPos(value * mMoveSpeed * mOwner->GetForward());
    }

    virtual void MoveRight(float value) override
    {
        V3 rightVector = glm::cross(mOwner->GetForward(), V3(0, 1, 0));
        mOwner->AddPos(value * mMoveSpeed * rightVector);
    }

    void TurnRight(float value) override
    {
        mOwner->AddYaw(mTurnSpeed * value);
    }

    void TurnUp(float value) override
    {
        float pitch = Math::Clamp(mOwner->GetPitch() + mTurnSpeed * value, - (float) M_PI_2, (float) M_PI_2);
        mOwner->SetPitch(pitch);
    }

private:
    float mMoveSpeed;
    float mTurnSpeed;
};

} // namespace vh
