#pragma once

#include "Core/Actor/Actor.hpp"
#include "Core/App/Module.hpp"
#include "Modules/World/ControlBehavior.hpp"
#include "Modules/Console/Console.hpp"

namespace vh {

/**
 * PlayerController handles input events from human.
 * PlayerController has a camera, that specifies what the human sees.
 */
class PlayerController : public Module
{
    VH_MODULE(PlayerController);

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