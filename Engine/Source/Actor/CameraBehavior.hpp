#pragma once

#include "ActorBehavior.hpp"

namespace vh
{

/*
    Provides a custom camera view for a controlled actor
*/
class CameraBehavior : public ActorBehavior
{
public:
    CameraBehavior(Actor* owner) : ActorBehavior(owner) {}

    virtual void TurnUp(float value) {};
    virtual M4 GetView();

    // -- get camera position
    virtual V3 GetPos();

    // -- get camera look direction
    virtual V3 GetForward();
};

} // namespace vh
