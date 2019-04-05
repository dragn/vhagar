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
class PlayerController : public Component
{
    VH_COMPONENT(PlayerController);

public:
    PlayerController();

    // Take possession of the object
    void Control(std::weak_ptr<Actor> actor);

    // Release previously possessed object
    void Release();

    virtual void HandleEvent(SDL_Event *event);

    virtual Ret TickInit(uint32_t delta) override;
    virtual Ret TickRun(uint32_t delta) override;

    std::weak_ptr<Actor> GetControlledActor()
    {
        return mActor;
    }

private:
    Console* mConsole;
    std::weak_ptr<Actor> mActor;

    float mCameraTurnSpeed;

    void _HandleKey(uint32_t type, SDL_KeyboardEvent *event);
    void _HandleMouse(int32_t xrel, int32_t yrel);

    bool mPressed[128];
};

} // namespace vh
