#pragma once

#include <vector>
#include <string>

#include "Modules/Console/ConsoleEngine.hpp"

namespace vh
{

class Console;

namespace ConsoleCommands
{

#define DECLARE_COMMAND(name) void ConsoleCommand_ ## name(const std::vector<std::string>&);
#define DEFINE_COMMAND(name) void ConsoleCommand_ ## name (const std::vector<std::string>& params)
#define REGISTER_COMMAND(name) App::Get<ConsoleEngine>()->Register(#name, ConsoleCommand_ ## name);

DECLARE_COMMAND(print);
DECLARE_COMMAND(list_actors);
DECLARE_COMMAND(quit);
DECLARE_COMMAND(help);
DECLARE_COMMAND(save_mesh);
DECLARE_COMMAND(load_mesh);
DECLARE_COMMAND(destroy_actor);
DECLARE_COMMAND(pos_actor);
DECLARE_COMMAND(move_actor);
DECLARE_COMMAND(spawn_mesh_actor);
DECLARE_COMMAND(rotate_actor);

void RegisterAll(ConsoleEngine* console);

} // namespace ConsoleCommands

} // namespace vh
