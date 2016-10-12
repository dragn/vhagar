#pragma once

#include "Actor.hpp"

namespace vh {

/**
 * Something that can be controlled by player or AI.
 */
class Controllable : public Actor {

public:
    Controllable() {}

    virtual void MoveForward(float value) = 0;
    virtual void MoveRight(float value) = 0;
    virtual void TurnRight(float value) = 0;
    virtual void TurnUp(float value) = 0;

};

}
