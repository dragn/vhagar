#pragma once

#include <vector>
#include <string>

#include "Console/Console.hpp"

namespace vh
{

class Console;

namespace ConsoleCommands
{

#define DECLARE_COMMAND(name) void ConsoleCommand_ ## name(const std::vector<std::string>&);
#define DEFINE_COMMAND(name) void ConsoleCommand_ ## name (const std::vector<std::string>& params)
#define REGISTER_COMMAND(name) App::Get<Console>()->Register(#name, ConsoleCommand_ ## name);

DECLARE_COMMAND(print);
DECLARE_COMMAND(list_actors);
DECLARE_COMMAND(quit);
DECLARE_COMMAND(help);

void RegisterAll(Console* console);

} // namespace ConsoleCommands

} // namespace vh
