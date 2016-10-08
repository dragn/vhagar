#include "World.hpp"
#include "Common.hpp"

namespace vh {

/*
void World::HandleEvent(SDL_Event *event) {
    CHECK(event);
    switch (event->type) {
        case SDL_MOUSEMOTION:
            mFreeCamera.TurnRight(event->motion.xrel);
            mFreeCamera.TurnUp(- event->motion.yrel);
            break;
    }
}

void World::Tick(Uint32 Delta) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    float Value = (float) Delta / 1000.0f;
    if (state[SDL_SCANCODE_W]) {
        mFreeCamera.MoveForward(Value);
    }
    if (state[SDL_SCANCODE_S]) {
        mFreeCamera.MoveForward(- Value);
    }
    if (state[SDL_SCANCODE_A]) {
        mFreeCamera.MoveRight(- Value);
    }
    if (state[SDL_SCANCODE_D]) {
        mFreeCamera.MoveRight(Value);
    }
}

void World::DoInit()
{
    if (mLevelMesh == nullptr && mRenderer->GetState() == vh::eRendererState_Ready) {
        // -- TODO: temporary. should be done with events.
        mLevelMesh = new vh::Mesh();

        vh::Utils::ImportWavefront(mLevelMesh, "Models/shadow_scene/shadow_scene.obj");
        mRenderer->AddObject(mLevelMesh);

        mLights.push_back({ V3(0, 2, 4), 0.5 });
        mLights.push_back({ V3(0, 3, -2), 0.3 });
        mLights.push_back({ V3(0, 1, -5), 0.3 });
        // --
    }
}
*/

} // namespace vh
