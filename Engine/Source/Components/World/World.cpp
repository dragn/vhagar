#include "Vhagar_PCH.hpp"

#include "World.hpp"
#include "App.hpp"
#include "Renderer/Renderer.hpp"
#include "Console/ConsoleCommands.hpp"
#include "Resource/ResourceSystem.hpp"
#include "RenderableBehavior/MeshBehavior.hpp"
#include "Utils/ImportUtils.hpp"
#include "App/PlayerController.hpp"
#include "Actor/CameraBehavior.hpp"
#include "Physics/Physics.hpp"

namespace vh
{

VH_COMPONENT_IMPL(World);

DEFINE_COMMAND(destroy_actor)
{
    if (params.size() <= 1)
    {
        LOG(INFO) << "Usage: " << params[0] << " <actor name>";
        return;
    }

    std::string name = params[1];
    std::weak_ptr<Actor> wActor = App::Get<World>()->GetActorByName(name);

    if (wActor.expired())
    {
        LOG(INFO) << "Actor " << name << " not found";
        return;
    }

    App::Get<World>()->DestroyActor(wActor.lock()->GetID());
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

    std::weak_ptr<Actor> wActor = App::Get<World>()->GetActorByName(params[1]);
    if (wActor.expired())
    {
        LOG(INFO) << "Actor " << params[1] << " not found";
        return;
    }

    float x = std::stof(params[2]);
    float y = std::stof(params[3]);
    float z = std::stof(params[4]);

    wActor.lock()->SetPos(V3(x, y, z));
}

DEFINE_COMMAND(move_actor)
{
    if (params.size() < 5)
    {
        LOG(INFO) << "Add position to actor";
        LOG(INFO) << "Usage: " << params[0] << " <name> <X> <Y> <Z>";
        return;
    }

    std::weak_ptr<Actor> wActor = App::Get<World>()->GetActorByName(params[1]);
    if (wActor.expired())
    {
        LOG(INFO) << "Actor " << params[1] << " not found";
        return;
    }

    float x = std::stof(params[2]);
    float y = std::stof(params[3]);
    float z = std::stof(params[4]);

    wActor.lock()->AddPos(V3(x, y, z));
}

DEFINE_COMMAND(rotate_actor)
{
    if (params.size() < 5)
    {
        LOG(INFO) << "Rotate actor";
        LOG(INFO) << "Usage: " << params[0] << " <name> <yaw> <pitch> <roll>";
    }

    std::weak_ptr<Actor> wActor = App::Get<World>()->GetActorByName(params[1]);
    if (wActor.expired())
    {
        LOG(INFO) << "Actor " << params[1] << " not found";
        return;
    }

    float x = std::stof(params[2]);
    float y = std::stof(params[3]);
    float z = std::stof(params[4]);

    wActor.lock()->AddRot(vh::Rot(x, y, z));
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

    std::weak_ptr<Actor> wActor = world->CreateActor("Mesh");

    CHECK(!wActor.expired());

    std::shared_ptr<Actor> sActor = wActor.lock();
    MeshBehavior* mb = sActor->AddBehavior<MeshBehavior>(params[1].c_str());
    if (mb->IsValid())
    {
        sActor->StartPlay();
    }
    else
    {
        world->DestroyActor(sActor->GetID());
    }
}

World::World() : Component(TickFrequency::NORMAL)
{
}

vh::Ret World::TickInit(uint32_t delta)
{
    Physics* physics = App::Get<Physics>();
    if (!physics || !physics->IsRunning()) return Ret::CONTINUE;

    mRenderer = App::Get<Renderer>();
    if (!mRenderer || !mRenderer->IsRunning()) return Ret::CONTINUE;

    REGISTER_COMMAND(destroy_actor);
    REGISTER_COMMAND(pos_actor);
    REGISTER_COMMAND(move_actor);
    REGISTER_COMMAND(spawn_mesh_actor);
    REGISTER_COMMAND(rotate_actor);

    return Ret::SUCCESS;
}

vh::Ret World::TickRun(uint32_t delta)
{
    // Tick each actor
    for (const std::shared_ptr<Actor>& actor : mActors)
    {
        actor->Tick(delta);
    }

    return Ret::CONTINUE;
}

vh::Ret World::TickClose(uint32_t delta)
{
    CHECK(mActors.empty()) << "the world is not empty";

    for (const std::shared_ptr<Actor>& actor : mActors)
    {
        actor->EndPlay();
    }
    // Destroy all actors
    mActors.clear();

    return Ret::SUCCESS;
}

void World::StartFrame()
{
    if (IsRunning() && mRenderer->IsRunning())
    {
        RenderBuffer* buffer = mRenderer->GetBufferHandler().GetNextBuffer();
        if (buffer)
        {
            buffer->header.size = 0;
            buffer->header.time = SDL_GetTicks();
            buffer->header.timestep = 16;

            PlayerController* controller = App::Get<PlayerController>();
            if (controller && !controller->GetControlledActor().expired())
            {
                CameraBehavior* camera = controller->GetControlledActor().lock()->GetBehaviorOfType<CameraBehavior>();
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
    if (IsRunning() && mRenderer->IsRunning())
    {
        RenderBuffer* buffer = mRenderer->GetBufferHandler().GetNextBuffer();
        if (buffer)
        {
            PlayerController* controller = App::Get<PlayerController>();
            if (controller && !controller->GetControlledActor().expired())
            {
                CameraBehavior* camera = controller->GetControlledActor().lock()->GetBehaviorOfType<CameraBehavior>();
                if (camera)
                {
                    buffer->header.view = camera->GetView();
                }
            }
        }
        mRenderer->GetBufferHandler().FlipBuffers();
    }
}

std::weak_ptr<vh::Actor> World::CreateActor(const std::string& name)
{
    std::string tmp(name);
    tmp.append("_");
    tmp.append(std::to_string(mActors.size()));

    std::shared_ptr<Actor> actor = std::make_shared<Actor>(GenerateActorID());

    // TODO fix root behavior initialization
    actor->mRootBehavior.Attach(actor, nullptr);

    mActors.push_back(actor);

    actor->SetName(tmp);

    return std::weak_ptr<Actor>(actor);
}

void World::ClearWorld()
{
    for (std::shared_ptr<Actor>& actor : mActors)
    {
        CHECK(actor.unique()) << "attempt to delete locked actor";
        actor->EndPlay();
    }

    mActors.clear();
}

} // namespace vh

