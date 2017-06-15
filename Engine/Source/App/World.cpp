#include "Common.hpp"

#include "World.hpp"
#include "App.hpp"
#include "Renderer/Renderer.hpp"
#include "Console/ConsoleCommands.hpp"
#include "Resource/ResourceSystem.hpp"
#include "Renderer/MeshBehavior.hpp"
#include "Utils/ImportUtils.hpp"
#include "App/PlayerController.hpp"
#include "Actor/CameraBehavior.hpp"
#include "Physics/Physics.hpp"

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

    Actor* actor = world->CreateActor("Mesh");
    MeshBehavior* mb = actor->AddBehavior<MeshBehavior>(params[1].c_str());
    if (mb->IsValid())
    {
        actor->StartPlay();
    }
    else
    {
        world->DestroyActor(actor);
    }
}

World::World() : Component(eTickFrequency::NORMAL)
{
    App::CheckRequired<Renderer>();
}

void World::TickInit(uint32_t delta)
{
    Physics* physics = App::Get<Physics>();
    if (!physics->IsRunning()) return;

    mRenderer = App::Get<Renderer>();
    if (!mRenderer->IsRunning()) return;

    REGISTER_COMMAND(destroy_actor);
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
    for (const std::unique_ptr<Actor>& actor : mActors)
    {
        actor->EndPlay();
    }
    // Destroy all actors
    mActors.clear();

    FinishClose();
}

void World::StartFrame()
{
    if (IsRunning())
    {
        RenderBuffer* buffer = mRenderer->GetBufferHandler().GetNextBuffer();
        if (buffer)
        {
            buffer->header.size = 0;
            buffer->header.time = SDL_GetTicks();
            buffer->header.timestep = 16;

            PlayerController* controller = App::Get<PlayerController>();
            if (controller && controller->GetControlledActor())
            {
                CameraBehavior* camera = controller->GetControlledActor()->GetBehaviorOfType<CameraBehavior>();
                if (camera)
                {
                    buffer->header.view = camera->GetView();
                }
            }
        }
    }
}

void World::EndFrame()
{
    if (IsRunning())
    {
        mRenderer->GetBufferHandler().FlipBuffers();
    }
}

} // namespace vh

