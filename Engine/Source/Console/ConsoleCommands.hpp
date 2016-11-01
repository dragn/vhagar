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

void RegisterAll(Console* console);

} // namespace ConsoleCommands

} // namespace vh
