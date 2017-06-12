#pragma once

#include "Actor/Actor.hpp"
#include "Component.hpp"
#include "Actor/ControlBehavior.hpp"
#include "Console/Console.hpp"

namespace vh {

/**
 * PlayerController handles input events from human.
 * PlayerController has a camera, that specifies what the human sees.
 */
class PlayerController : public Component {
    COMPONENT_NAME(PlayerController);

public:
    PlayerController();

    // Take possession of the object
    void Control(Actor *actor);

    // Release previously possessed object
    void Release();

    virtual void HandleEvent(SDL_Event *event);

    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    Actor* GetControlledActor()
    {
        return mActor;
    }

private:
    Console* mConsole;
    Actor* mActor;

    float mCameraTurnSpeed;

    void _HandleKey(uint32_t type, SDL_KeyboardEvent *event);
    void _HandleMouse(int32_t xrel, int32_t yrel);

    bool mPressed[128];
};

} // namespace vh
