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
    virtual void TurnUp(float value) {};
    M4 GetView();

    // -- get camera look direction
    V3 GetForward();
};

} // namespace vh
