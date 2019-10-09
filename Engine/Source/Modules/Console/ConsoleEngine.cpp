#include "Modules/VhModules_PCH.hpp"
#include "ConsoleCommands.hpp"

#include "Modules/Console/ConsoleEngine.hpp"

namespace vh
{

VH_MODULE_IMPL(ConsoleEngine)

vh::Ret ConsoleEngine::TickInit(uint32_t delta)
{
    ConsoleCommands::RegisterAll(this);

    return Ret::SUCCESS;
}

vh::Ret ConsoleEngine::TickRun(uint32_t delta)
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

void ConsoleEngine::Register(const std::string& name, CmdHandler handler)
{
    mCommands[name] = handler;
}

void ConsoleEngine::_Exec(const std::string& cmd)
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

void ConsoleEngine::Exec(const std::string& cmd)
{
    cs::CritSectionLock lock(mCmdQueueCS);

    mCmdQueue.push(cmd);
}

void ConsoleEngine::PrintHelp()
{
    LOG(INFO) << "Commands list:";
    for (const std::pair<std::string, CmdHandler>& cmd : mCommands)
    {
        LOG(INFO) << "  " << cmd.first;
    }
}

} // namespace vh
