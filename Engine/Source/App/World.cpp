#include "Common.hpp"

#include "World.hpp"
#include "App.hpp"
#include "Renderer/Renderer.hpp"
#include "Console/ConsoleCommands.hpp"

namespace vh {

DEFINE_COMMAND(destroy_actor)
{
    if (params.size() <= 1)
    {
        LOG(INFO) << "Usage: " << params[0] << " <actor name>";
        return;
    }

    std::string name = params[1];
    Actor* actor = App::Get<World>()->GetActorByName<Actor>(name);
    if (actor == nullptr)
    {
        LOG(INFO) << "Actor " << name << " not found";
        return;
    }

    App::Get<World>()->DestroyActor(actor);
    LOG(INFO) << "Actor " << name << " destroyed";
}

DEFINE_COMMAND(spawn_actor)
{
    if (params.size() <= 1)
    {
        LOG(INFO) << "Usage: " << params[0] << " <actor class name>";
        return;
    }

    App::Get<World>()->SpawnActorByClassName(params[1]);
}

DEFINE_COMMAND(pos_actor)
{
    if (params.size() < 5)
    {
        LOG(INFO) << "Set new position for actor";
        LOG(INFO) << "Usage: " << params[0] << " <name> <X> <Y> <Z>";
        return;
    }

    Actor* actor = App::Get<World>()->GetActorByName<Actor>(params[1]);
    if (actor == nullptr)
    {
        LOG(INFO) << "Actor " << params[1] << " not found";
        return;
    }

    float x = std::stof(params[2]);
    float y = std::stof(params[3]);
    float z = std::stof(params[4]);

    actor->SetPos(V3(x, y, z));
}

const char* World::COMPONENT_NAME = "WORLD";

World::World() : Component(COMPONENT_NAME, 16)
{
    App::CheckRequired<Renderer>();
}

void World::TickInit(uint32_t delta)
{
    REGISTER_COMMAND(destroy_actor);
    REGISTER_COMMAND(spawn_actor);
    REGISTER_COMMAND(pos_actor);

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

