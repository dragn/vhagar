#include "Common.hpp"

#include "App/World.hpp"
#include "App/App.hpp"

#include "ConsoleCommands.hpp"
#include "Console.hpp"

#include "Resource/ResourceSystem.hpp"
#include "Actor/StaticMeshActor.hpp"

namespace vh
{

#define REGISTER_COMMAND(name) console->Register(#name, ConsoleCommand_ ## name);

namespace ConsoleCommands
{

void RegisterAll(ConsoleEngine* console)
{
    CHECK(console);

    REGISTER_COMMAND(print);
    REGISTER_COMMAND(list_actors);
    REGISTER_COMMAND(quit);
    REGISTER_COMMAND(help);
    REGISTER_COMMAND(save_mesh);
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

    StaticMeshActor* actor = world->GetActorByName<StaticMeshActor>(params[1]);
    if (actor == nullptr)
    {
        LOG(INFO) << "Could not find actor " << params[1];
        return;
    }

    if (rs->Save<Mesh>(params[2].c_str(), actor->GetMesh()))
    {
        LOG(INFO) << "Saved mesh successfully";
    }
}

} // namespace ConsoleCommands

} // namespace vh
