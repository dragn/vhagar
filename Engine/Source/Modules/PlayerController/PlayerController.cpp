#include "Vhagar_PCH.hpp"

#include "PlayerController.hpp"
#include "Core/App/App.hpp"
#include "Modules/Renderer/Renderer.hpp"
#include "Core/Modules/MWorld.hpp"
#include "Modules/World/CameraBehavior.hpp"

namespace vh
{

VH_MODULE_IMPL(PlayerController);

PlayerController::PlayerController()
    : Module(TickFrequency::NORMAL)
    , mCameraTurnSpeed(0.001f)
{
}

vh::Ret PlayerController::TickInit(uint32_t delta)
{
    App::CheckRequired<Renderer>();
    App::CheckRequired<MWorld>();

    mConsole = App::Get<Console>();
    if (!mConsole || !mConsole->IsRunning())
    {
        return Ret::CONTINUE;
    }

    return Ret::SUCCESS;
}

vh::Ret PlayerController::TickRun(uint32_t delta)
{
    if (mActor.expired()) return Ret::CONTINUE;

    // do nothing if console is open
    if (mConsole->IsShown()) return Ret::CONTINUE;

    float sec = delta / 1000.0f;

    mActor.lock()->ForEachBehaviorOfType<ControlBehavior>([this, sec] (ControlBehavior* behavior)
    {
        if (mPressed['w']) behavior->MoveForward(1.0f);
        if (mPressed['s']) behavior->MoveForward(-1.0f);
        if (mPressed['a']) behavior->MoveRight(-1.0f);
        if (mPressed['d']) behavior->MoveRight(1.0f);
    });

    return Ret::CONTINUE;
}

void PlayerController::Control(std::weak_ptr<Actor> actor)
{
    mActor = actor;
    for (bool &b : mPressed) b = false;
}

void PlayerController::HandleEvent(SDL_Event *event)
{
    if (mActor.expired()) return;

    switch (event->type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        _HandleKey(event->type, &event->key);
        break;

    case SDL_MOUSEMOTION:
        _HandleMouse(event->motion.xrel, event->motion.yrel);
        break;

    case SDL_MOUSEBUTTONDOWN:
        /*
        if (event->button.button == SDL_BUTTON_X2) {
            if (mCameraBoom.GetLength() < 40.0f) mCameraBoom.AddLength(2.0f);
        } else if (event->button.button == SDL_BUTTON_X1) {
            if (mCameraBoom.GetLength() > 10.0f) mCameraBoom.AddLength(-2.0f);
        }
        */
        break;
    }
}

void PlayerController::_HandleKey(uint32_t type, SDL_KeyboardEvent *event)
{
    SDL_Keycode k = event->keysym.sym;
    if (k > 127) return; // handle only symbol keys for now...
    switch (type)
    {
    case SDL_KEYDOWN:
        mPressed[k] = true;
        break;
    case SDL_KEYUP:
        mPressed[k] = false;
        break;
    }
}

void PlayerController::_HandleMouse(int32_t xrel, int32_t yrel)
{
    if (mActor.expired()) return;

    mActor.lock()->ForEachBehaviorOfType<ControlBehavior>([xrel, yrel] (ControlBehavior* behavior)
    {
        behavior->TurnRight(static_cast<float>(xrel));
        behavior->TurnUp(static_cast<float>(-yrel));
    });
}

} // namespace vh
