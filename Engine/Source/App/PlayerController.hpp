#pragma once

#include "Actor/Actor.hpp"
#include "Arm.hpp"
#include "Component.hpp"
#include "Actor/Controllable.hpp"
#include "Actor/CameraActor.hpp"
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
    void Control(Controllable *actor);

    // Release previously possessed object
    void Release();

    virtual void HandleEvent(SDL_Event *event);

    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

private:
    Console* mConsole;
    Controllable* mActor;

    CameraActor* mCamera;
    //Arm* mCameraBoom;

    float mCameraTurnSpeed;

    void _HandleKey(uint32_t type, SDL_KeyboardEvent *event);
    void _HandleMouse(int32_t xrel, int32_t yrel);

    bool mPressed[128];
};

} // namespace vh
