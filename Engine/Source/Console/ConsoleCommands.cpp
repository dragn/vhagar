#include "Vhagar_PCH.hpp"

#include "App/World.hpp"
#include "App/App.hpp"

#include "ConsoleCommands.hpp"
#include "Console.hpp"

#include "Resource/ResourceSystem.hpp"

#include "RenderableBehavior/MeshBehavior.hpp"

namespace vh
{

#define REGISTER_COMMAND_LOCAL(name) console->Register(#name, ConsoleCommand_ ## name);

namespace ConsoleCommands
{

void RegisterAll(ConsoleEngine* console)
{
    CHECK(console);

    REGISTER_COMMAND_LOCAL(print);
    REGISTER_COMMAND_LOCAL(list_actors);
    REGISTER_COMMAND_LOCAL(quit);
    REGISTER_COMMAND_LOCAL(help);
    REGISTER_COMMAND_LOCAL(save_mesh);
}

DEFINE_COMMAND(print)
{
    std::string str;
    for (size_t idx = 1; idx < params.size(); ++idx) str.append(params[idx]);

    LOG(INFO) << str;
}

DEFINE_COMMAND(list_actors)
{
    World* world = App::Get<World>();
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
    App::Get<ConsoleEngine>()->PrintHelp();
}

DEFINE_COMMAND(save_mesh)
{
    ResourceSystem* rs = App::Get<ResourceSystem>();
    World* world = App::Get<World>();

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
    actor.lock()->ForEachBehaviorOfType<MeshBehavior>([rs, filename] (MeshBehavior* behavior)
    {
        if (behavior->Get())
        {
            if (rs->Save<Mesh>(filename, behavior->Get()))
            {
                LOG(INFO) << "Saved mesh successfully";
            }
        }
    });
}

} // namespace ConsoleCommands

} // namespace vh
