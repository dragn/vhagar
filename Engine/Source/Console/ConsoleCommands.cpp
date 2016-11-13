#include "Common.hpp"

#include "App/World.hpp"
#include "App/App.hpp"

#include "ConsoleCommands.hpp"
#include "Console.hpp"

namespace vh
{

#define REGISTER_COMMAND(name) console->Register(#name, ConsoleCommand_ ## name);
#define DEFINE_COMMAND(name) void ConsoleCommand_ ## name (const std::vector<std::string>& params)

namespace ConsoleCommands
{

void RegisterAll(Console* console)
{
    CHECK(console);

    REGISTER_COMMAND(print);
    REGISTER_COMMAND(list_actors);
}

DEFINE_COMMAND(print)
{
    LOG(INFO) << "Console: " << params[0];
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

} // namespace ConsoleCommands

} // namespace vh
