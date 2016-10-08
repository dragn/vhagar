#include "PlayerController.hpp"
#include "Common.hpp"

namespace vh {

void PlayerController::Control(Actor *actor) {
    mActor = actor;
    for (bool &b : mPressed) b = false;
    mCameraBoom.SetLength(20.0f);
    _UpdateCameraPos();
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
            if (event->button.button == SDL_BUTTON_X2) {
                if (mCameraBoom.GetLength() < 40.0f) mCameraBoom.AddLength(2.0f);
            } else if (event->button.button == SDL_BUTTON_X1) {
                if (mCameraBoom.GetLength() > 10.0f) mCameraBoom.AddLength(-2.0f);
            }
            _UpdateCameraPos();
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

void PlayerController::_UpdateCameraPos() {
    if (mActor == nullptr) return;

    mCameraBoom.SetPos(mActor->GetPos());

    mCamera.SetPos(mCameraBoom.GetPos() - mCameraBoom.GetForward() * mCameraBoom.GetLength());

    mCamera.SetRot(mCameraBoom.GetRot());
}

void PlayerController::_HandleMouse(int32_t xrel, Sint32 yrel) {
    if (mActor == nullptr) return;

    mCameraBoom.AddYaw(- xrel * mCameraTurnSpeed);
    mCameraBoom.AddPitch(- yrel * mCameraTurnSpeed);

    _UpdateCameraPos();
}

void PlayerController::Tick(uint32_t delta) {
    if (mActor == nullptr) return;

    //float sec = delta / 1000.0f;
    if (mPressed['w']) {
        _UpdateCameraPos();
    }
    //  if (mPressed['s']) mActor->moveForward(-sec);
    //  if (mPressed['a']) mActor->moveRight(-sec);
    //  if (mPressed['d']) mActor->moveRight(sec);

}

} // namespace vh
