#pragma once

#include "../Actor/Actor.hpp"
#include "Arm.hpp"

namespace vh {

/**
 * PlayerController handles input events from human.
 * PlayerController has a camera, that specifies what the human sees.
 */
class PlayerController {

public:
    PlayerController()
        : mActor(nullptr)
        , mCameraTurnSpeed(0.001f)
    {}

    // Take possession of the object
    void Control(Actor *actor);

    // Release previously possessed object
    void Release();

    void HandleEvent(SDL_Event *event);
    void Tick(uint32_t delta);

    const Actor& GetCamera()
    {
        return mCamera;
    }

private:
    Actor *mActor;

    Actor mCamera;
    Arm mCameraBoom;

    float mCameraTurnSpeed;

    void _UpdateCameraPos();
    void _HandleKey(uint32_t type, SDL_KeyboardEvent *event);
    void _HandleMouse(int32_t xrel, int32_t yrel);

    bool mPressed[128];
};

} // namespace vh
