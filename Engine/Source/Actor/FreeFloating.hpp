#pragma once

#include "Math/Math.hpp"
#include "Controllable.hpp"

namespace vh
{

class FreeFloating : public Controllable
{
public:
    FreeFloating() : mMoveSpeed(5), mTurnSpeed(0.001f) {}

    virtual void MoveForward(float value)
    {
        AddPos(value * mMoveSpeed * GetForward());
    }

    virtual void MoveRight(float value)
    {
        V3 rightVector = glm::cross(GetForward(), V3(0, 1, 0));
        AddPos(value * mMoveSpeed * rightVector);
    }

    void TurnRight(float value)
    {
        AddYaw(mTurnSpeed * value);
    }

    void TurnUp(float value)
    {
        float pitch = Math::Clamp(GetPitch() + mTurnSpeed * value, - (float) M_PI_2, (float) M_PI_2);
        SetPitch(pitch);
    }

private:
    float mMoveSpeed;
    float mTurnSpeed;
};

} // namespace vh
