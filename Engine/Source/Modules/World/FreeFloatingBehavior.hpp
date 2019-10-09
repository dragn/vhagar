#pragma once

#include "Modules/standard.hpp"
#include "Modules/World/ControlBehavior.hpp"

namespace vh
{

class FreeFloatingBehavior : public ControlBehavior
{
public:
    FreeFloatingBehavior()
        : mMoveSpeed(5)
        , mTurnSpeed(0.001f)
    {}

    virtual void MoveForward(float value) override
    {
        LockOwner()->AddPos(value * mMoveSpeed * LockOwner()->GetForward());
    }

    virtual void MoveRight(float value) override
    {
        V3 rightVector = glm::cross(LockOwner()->GetForward(), V3(0, 1, 0));
        LockOwner()->AddPos(value * mMoveSpeed * rightVector);
    }

    void TurnRight(float value) override
    {
        LockOwner()->AddYaw(- mTurnSpeed * value);
    }

    void TurnUp(float value) override
    {
        float pitch = Math::Clamp(LockOwner()->GetPitch() + mTurnSpeed * value, - (float) M_PI_2, (float) M_PI_2);
        LockOwner()->SetPitch(pitch);
    }

private:
    float mMoveSpeed;
    float mTurnSpeed;
};

} // namespace vh
