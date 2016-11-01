#include "Console.hpp"
#include <sstream>
#include <iterator>

namespace vh
{

const char* Console::COMPONENT_NAME = "Console";

void Console::TickInit(uint32_t delta)
{
    ConsoleCommands::RegisterAll(this);

    FinishInit();
}

void Console::TickRun(uint32_t delta)
{
    if (!mCmdQueue.empty())
    {
        cs::CritSectionLock lock(mCmdQueueCS);

        while (!mCmdQueue.empty())
        {
            const std::string& cmd = mCmdQueue.front();
            _Exec(cmd);
            mCmdQueue.pop();
        }
    }
}

void Console::TickClose(uint32_t delta)
{
    FinishClose();
}

void Console::Register(const std::string& name, CmdHandler handler)
{
    mCommands[name] = handler;
}

void Console::_Exec(const std::string& cmd)
{
    std::stringstream ss(cmd);

    std::istream_iterator<std::string> ss_iter(ss);
    std::istream_iterator<std::string> end;

    std::vector<std::string> params(ss_iter, end);

    if (params.size() == 0)
    {
        LOG(WARNING) << "Invalid command: '" << cmd << "'";
        return;
    }

    CmdMap::iterator entry = mCommands.find(params[0]);
    if (entry == mCommands.end())
    {
        LOG(WARNING) << "Command handler not found: " << params[0];
        return;
    }

    entry->second(params);
}

void Console::Exec(const std::string& cmd)
{
    cs::CritSectionLock lock(mCmdQueueCS);

    mCmdQueue.push(cmd);
}

} // namespace vh
