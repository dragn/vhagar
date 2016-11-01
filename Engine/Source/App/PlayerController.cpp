#include "PlayerController.hpp"
#include "Common.hpp"
#include "App.hpp"
#include "Renderer/Renderer.hpp"
#include "World.hpp"
#include <glm/gtx/transform.hpp>

namespace vh {

const char* PlayerController::COMPONENT_NAME = "PlayerController";

PlayerController::PlayerController() :
    Component(COMPONENT_NAME, 16),
    mActor(nullptr),
    mCamera(nullptr),
    mCameraTurnSpeed(0.001f)
{
    if (App::GetComponent<Renderer>() == nullptr)
    {
        LOG(FATAL) << "Missing required component: " << Renderer::COMPONENT_NAME;
    }
    if (App::GetComponent<World>() == nullptr)
    {
        LOG(FATAL) << "Missing required component: " << World::COMPONENT_NAME;
    }
}

void PlayerController::TickInit(uint32_t delta)
{
    mCamera = App::GetComponent<World>()->SpawnActor<Actor>();

    FinishInit();
}

void PlayerController::TickRun(uint32_t delta)
{
    if (mActor == nullptr) return;

    float sec = delta / 1000.0f;
    if (mPressed['w']) mActor->MoveForward(sec);
    if (mPressed['s']) mActor->MoveForward(-sec);
    if (mPressed['a']) mActor->MoveRight(-sec);
    if (mPressed['d']) mActor->MoveRight(sec);

    // Update camera position
    mCamera->SetPos(mActor->GetPos());
    mCamera->SetRot(mActor->GetRot());

    M4 view = glm::lookAt(mCamera->GetPos(), mCamera->GetPos() + mCamera->GetForward(), mCamera->GetUp());
    App::GetComponent<Renderer>()->SetView(view);
}

void PlayerController::TickClose(uint32_t delta)
{
    App::GetComponent<World>()->DestroyActor(mCamera);

    FinishClose();
}

void PlayerController::Control(Controllable *actor) {
    mActor = actor;
    for (bool &b : mPressed) b = false;
}

void PlayerController::HandleEvent(SDL_Event *event) {

    if (mActor == nullptr) return;

    switch (event->type) {
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

void PlayerController::_HandleKey(uint32_t type, SDL_KeyboardEvent *event) {
    SDL_Keycode k = event->keysym.sym;
    if (k > 127) return; // handle only symbol keys for now...
    switch (type) {
        case SDL_KEYDOWN:
            mPressed[k] = true;
            break;
        case SDL_KEYUP:
            mPressed[k] = false;
            break;
    }
}

void PlayerController::_HandleMouse(int32_t xrel, int32_t yrel) {
    if (mActor == nullptr) return;

    mActor->TurnRight(xrel);
    mActor->TurnUp(-yrel);
}

} // namespace vh