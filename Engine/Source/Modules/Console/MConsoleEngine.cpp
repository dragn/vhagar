#include "Modules/VhModules_PCH.hpp"
#include "ConsoleCommands.hpp"

#include "Modules/Console/MConsoleEngine.hpp"

namespace vh
{

VH_MODULE_IMPL(MConsoleEngine)

vh::Ret MConsoleEngine::TickInit(uint32_t delta)
{
    ConsoleCommands::RegisterAll(this);

    return Ret::SUCCESS;
}

vh::Ret MConsoleEngine::TickRun(uint32_t delta)
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

    return Ret::CONTINUE;
}

void MConsoleEngine::Register(const std::string& name, CmdHandler handler)
{
    mCommands[name] = handler;
}

void MConsoleEngine::_Exec(const std::string& cmd)
{
    LOG(INFO) << "> " << cmd;

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

void MConsoleEngine::Exec(const std::string& cmd)
{
    cs::CritSectionLock lock(mCmdQueueCS);

    mCmdQueue.push(cmd);
}

void MConsoleEngine::PrintHelp()
{
    LOG(INFO) << "Commands list:";
    for (const std::pair<std::string, CmdHandler>& cmd : mCommands)
    {
        LOG(INFO) << "  " << cmd.first;
    }
}

} // namespace vh
