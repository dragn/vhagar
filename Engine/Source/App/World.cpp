#include "Common.hpp"

#include "World.hpp"
#include "App.hpp"
#include "Renderer/Renderer.hpp"

namespace vh {

const char* World::COMPONENT_NAME = "WORLD";

World::World() : Component(COMPONENT_NAME, 16)
{
    if (App::GetComponent<Renderer>() == nullptr)
    {
        LOG(FATAL) << "Missing required component: " << Renderer::COMPONENT_NAME;
    }
}

void World::TickInit(uint32_t delta)
{
    FinishInit();
}

void World::TickRun(uint32_t delta)
{
    // Tick each actor
    for (Actor* actor : mActors)
    {
        actor->Tick(delta);
    }
}

void World::TickClose(uint32_t delta)
{
    // Destroy all actors
    for (Actor* actor : mActors)
    {
        actor->OnDestroy();
        delete actor;
    }
    mActors.clear();

    FinishClose();
}

} // namespace vh

