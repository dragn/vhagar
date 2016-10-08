#pragma once

namespace vh {

/**
 * Something that can be controlled by player or AI.
 */
class Controllable : public Actor {

public:

    Controllable() : mMoveSpeed(5), mTurnSpeed(0.003) {}

    void MoveForward(float value) {
        AddPos(value * mMoveSpeed * GetForward());
    }
    void MoveRight(float value) {
        V3 rightVector = glm::cross(GetForward(), V3(0, 1, 0));
        AddPos(value * mMoveSpeed * rightVector);
    }
    void TurnRight(float value) {
        AddYaw(mTurnSpeed * value);
    }
    void TurnUp(float value) {
        AddPitch(mTurnSpeed * value);
    }

private:
    float mMoveSpeed;
    float mTurnSpeed;

};

}
