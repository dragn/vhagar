#include "Modules/VhModules_PCH.hpp"
#include "ConsoleCommands.hpp"

#include "Modules/Console/MConsoleGUI.hpp"
#include "Modules/ResourceSystem/MResourceSystem.hpp"
#include "Modules/Renderer/RenderableBehavior/BMesh.hpp"

namespace vh
{

#define REGISTER_COMMAND_LOCAL(name) console->Register(#name, ConsoleCommand_ ## name);

namespace ConsoleCommands
{

void RegisterAll(MConsoleEngine* console)
{
    CHECK(console);

    REGISTER_COMMAND_LOCAL(print);
    REGISTER_COMMAND_LOCAL(list_actors);
    REGISTER_COMMAND_LOCAL(quit);
    REGISTER_COMMAND_LOCAL(help);
    REGISTER_COMMAND_LOCAL(save_mesh);
    REGISTER_COMMAND_LOCAL(destroy_actor);
    REGISTER_COMMAND_LOCAL(pos_actor);
    REGISTER_COMMAND_LOCAL(move_actor);
    REGISTER_COMMAND_LOCAL(spawn_mesh_actor);
    REGISTER_COMMAND_LOCAL(rotate_actor);
}

DEFINE_COMMAND(print)
{
    std::string str;
    for (size_t idx = 1; idx < params.size(); ++idx) str.append(params[idx]);

    LOG(INFO) << str;
}

DEFINE_COMMAND(list_actors)
{
    MWorld* world = App::Get<MWorld>();
    if (world)
    {
        LOG(INFO) << "Actor list:";
        for (auto actor = world->GetActors().begin(); actor != world->GetActors().end(); ++actor)
        {
            LOG(INFO) << " - " << (*actor)->GetName() << std::endl;
        }
    }
    else
    {
        LOG(INFO) << "Component World is not loaded";
    }
}

DEFINE_COMMAND(quit)
{
    GetApp()->Close();
}

DEFINE_COMMAND(help)
{
    App::Get<MConsoleEngine>()->PrintHelp();
}

DEFINE_COMMAND(save_mesh)
{
    MResourceSystem* rs = App::Get<MResourceSystem>();
    MWorld* world = App::Get<MWorld>();

    CHECK(world);
    CHECK(rs);

    if (params.size() < 3)
    {
        LOG(INFO) << "Usage: " << params[0] << " <actor name> <file name>";
        return;
    }

    std::weak_ptr<Actor> actor = world->GetActorByName(params[1]);
    if (actor.expired())
    {
        LOG(INFO) << "Could not find actor " << params[1];
        return;
    }

    const char* filename = params[2].c_str();
    actor.lock()->ForEachBehaviorOfType<BMesh>([rs, filename] (BMesh* behavior)
    {
        /*
        if (behavior->GetMesh())
        {
            if (rs->Save<Mesh>(filename, behavior->GetMesh()))
            {
                LOG(INFO) << "Saved mesh successfully";
            }
        }
        */
    });
}


DEFINE_COMMAND(destroy_actor)
{
    if (params.size() <= 1)
    {
        LOG(INFO) << "Usage: " << params[0] << " <actor name>";
        return;
    }

    std::string name = params[1];
    std::weak_ptr<Actor> wActor = App::Get<MWorld>()->GetActorByName(name);

    if (wActor.expired())
    {
        LOG(INFO) << "Actor " << name << " not found";
        return;
    }

    App::Get<MWorld>()->DestroyActor(wActor.lock()->GetID());
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

    std::weak_ptr<Actor> wActor = App::Get<MWorld>()->GetActorByName(params[1]);
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

    std::weak_ptr<Actor> wActor = App::Get<MWorld>()->GetActorByName(params[1]);
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

    std::weak_ptr<Actor> wActor = App::Get<MWorld>()->GetActorByName(params[1]);
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
    MResourceSystem* rs = App::Get<MResourceSystem>();
    MWorld* world = App::Get<MWorld>();

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
    BMesh* mb = sActor->AddBehavior<BMesh>(params[1].c_str());
    sActor->StartPlay();
}

} // namespace ConsoleCommands

} // namespace vh
