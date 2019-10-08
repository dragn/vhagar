#pragma once

#include "ActorBehavior.hpp"

namespace vh
{

/*
    Basic behavior for controllable actors
*/
class ControlBehavior : public ActorBehavior
{
public:
    virtual void MoveForward(float value) = 0;
    virtual void MoveRight(float value) = 0;
    virtual void TurnRight(float value) = 0;
    virtual void TurnUp(float value) = 0;
};

} // namespace vh
