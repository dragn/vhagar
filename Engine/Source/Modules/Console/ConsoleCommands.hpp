#pragma once

#include "Modules/standard.hpp"
#include "Modules/Console/MConsoleEngine.hpp"

namespace vh
{

class MConsoleGUI;

namespace ConsoleCommands
{

#define DECLARE_COMMAND(name) void ConsoleCommand_ ## name(const std::vector<std::string>&);
#define DEFINE_COMMAND(name) void ConsoleCommand_ ## name (const std::vector<std::string>& params)
#define REGISTER_COMMAND(name) App::Get<MConsoleEngine>()->Register(#name, ConsoleCommand_ ## name);

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

void RegisterAll(MConsoleEngine* console);

} // namespace ConsoleCommands

} // namespace vh
