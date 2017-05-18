#include "Common.hpp"

#include "World.hpp"
#include "App.hpp"
#include "Renderer/Renderer.hpp"
#include "Console/ConsoleCommands.hpp"
#include "Resource/ResourceSystem.hpp"
#include "Actor/StaticMeshActor.hpp"
#include "Utils/ImportUtils.hpp"

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

DEFINE_COMMAND(move_actor)
{
    if (params.size() < 5)
    {
        LOG(INFO) << "Add position to actor";
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

    actor->AddPos(V3(x, y, z));
}

DEFINE_COMMAND(rotate_actor)
{
    if (params.size() < 5)
    {
        LOG(INFO) << "Rotate actor";
        LOG(INFO) << "Usage: " << params[0] << " <name> <yaw> <pitch> <roll>";
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

    actor->AddRot(vh::Rot(x, y, z));
}

DEFINE_COMMAND(spawn_mesh_actor)
{
    ResourceSystem* rs = App::Get<ResourceSystem>();
    World* world = App::Get<World>();

    CHECK(world);
    CHECK(rs);

    if (params.size() < 2)
    {
        LOG(INFO) << "Usage: " << params[0] << " <file name>";
        return;
    }

    Mesh* mesh = new Mesh;
    if (rs->Load<Mesh>(params[1].c_str(), mesh))
    {
        LOG(INFO) << "Mesh loaded successfully";

        StaticMeshActor* actor = world->SpawnActor<StaticMeshActor>();
        actor->SetMesh(mesh);
    }
}

World::World() : Component(eTickFrequency::NORMAL)
{
    App::CheckRequired<Renderer>();
}

void World::TickInit(uint32_t delta)
{
    REGISTER_COMMAND(destroy_actor);
    REGISTER_COMMAND(spawn_actor);
    REGISTER_COMMAND(pos_actor);
    REGISTER_COMMAND(move_actor);
    REGISTER_COMMAND(spawn_mesh_actor);
    REGISTER_COMMAND(rotate_actor);

    FinishInit();
}

void World::TickRun(uint32_t delta)
{
    // Tick each actor
    for (const std::unique_ptr<Actor>& actor : mActors)
    {
        actor->Tick(delta);
    }
}

void World::TickClose(uint32_t delta)
{
    // Destroy all actors
    for (const std::unique_ptr<Actor>& actor : mActors)
    {
        actor->OnDestroy();
    }
    mActors.clear();

    FinishClose();
}

} // namespace vh

