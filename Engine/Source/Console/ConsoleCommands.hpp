#pragma once

#include <vector>
#include <string>

namespace vh
{

class Console;

namespace ConsoleCommands
{

#define DECLARE_COMMAND(name) void ConsoleCommand_ ## name(const std::vector<std::string>&);

DECLARE_COMMAND(print);
DECLARE_COMMAND(list_actors);

void RegisterAll(Console* console);

} // namespace ConsoleCommands

} // namespace vh
