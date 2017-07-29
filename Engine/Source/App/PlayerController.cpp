#include "Common.hpp"

#include "PlayerController.hpp"
#include "App.hpp"
#include "Renderer/Renderer.hpp"
#include "World.hpp"
#include "Actor/CameraBehavior.hpp"

namespace vh
{

VH_COMPONENT_IMPL(PlayerController);

PlayerController::PlayerController() :
    Component(eTickFrequency::NORMAL),
    mActor(nullptr),
    mCameraTurnSpeed(0.001f)
{
    App::CheckRequired<Renderer>();
    App::CheckRequired<World>();
}

void PlayerController::TickInit(uint32_t delta)
{
    mConsole = App::Get<Console>();

    FinishInit();
}

void PlayerController::TickRun(uint32_t delta)
{
    if (mActor == nullptr) return;

    // do nothing if console is open
    if (mConsole->IsShown()) return;

    float sec = delta / 1000.0f;

    mActor->ForEachBehaviorOfType<ControlBehavior>([this, sec] (ControlBehavior* behavior)
    {
        if (mPressed['w']) behavior->MoveForward(sec);
        if (mPressed['s']) behavior->MoveForward(-sec);
        if (mPressed['a']) behavior->MoveRight(-sec);
        if (mPressed['d']) behavior->MoveRight(sec);
    });

    // Update renderer view with Actor's custom or default camera behavior
    CameraBehavior* camera = mActor->GetBehaviorOfType<CameraBehavior>();
    if (camera != nullptr)
    {
        App::Get<Renderer>()->SetView(camera->GetView());
    }
    else
    {
        // no camera behavior, use actor's view
        App::Get<Renderer>()->SetView(mActor->GetView());
    }
}

void PlayerController::TickClose(uint32_t delta)
{
    FinishClose();
}

void PlayerController::Control(Actor *actor)
{
    mActor = actor;
    for (bool &b : mPressed) b = false;
}

void PlayerController::HandleEvent(SDL_Event *event)
{

    if (mActor == nullptr) return;

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
    if (mActor == nullptr) return;

    mActor->ForEachBehaviorOfType<ControlBehavior>([xrel, yrel] (ControlBehavior* behavior)
    {
        behavior->TurnRight(static_cast<float>(xrel));
        behavior->TurnUp(static_cast<float>(-yrel));
    });
}

} // namespace vh
