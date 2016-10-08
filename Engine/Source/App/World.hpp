#pragma once

#include "Common.hpp"
#include "Actor/Actor.hpp"
#include "Renderer/LightSource.hpp"

namespace vh {

class World {

public:
    World() {};

    void DoInit();

    void HandleEvent(SDL_Event *event);
    void Tick(Uint32 delta);

private:
    std::vector<Actor*> mActors;

    UNCOPYABLE(World);
};

} // namespace vh
