#include "Common.hpp"

#include "App/World.hpp"
#include "App/App.hpp"

#include "ConsoleCommands.hpp"
#include "Console.hpp"

namespace vh
{

#define REGISTER_COMMAND(name) console->Register(#name, ConsoleCommand_ ## name);

namespace ConsoleCommands
{

void RegisterAll(Console* console)
{
    CHECK(console);

    REGISTER_COMMAND(print);
    REGISTER_COMMAND(list_actors);
    REGISTER_COMMAND(quit);
    REGISTER_COMMAND(help);
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
    App::Get<Console>()->PrintHelp();
}

} // namespace ConsoleCommands

} // namespace vh
