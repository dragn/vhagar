#include "Common.hpp"

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
}

void ConsoleCommand_print(const std::vector<std::string>& params)
{
    LOG(INFO) << "Console: " << params[0];
}

} // namespace ConsoleCommands

} // namespace vh
