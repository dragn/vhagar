#include "Common.hpp"

#include "Console.hpp"
#include <sstream>
#include <iterator>

namespace vh
{

const char* Console::COMPONENT_NAME = "Console";

class ConsoleLogSink : public google::LogSink
{
public:
    ConsoleLogSink(Console* console)
        : mConsole(console)
    {
        CHECK(console);
    }

    virtual void send(google::LogSeverity severity, const char* full_filename,
        const char* base_filename, int line,
        const struct ::tm* tm_time,
        const char* message, size_t message_len)
    {
        mConsole->PrintMessage(ToString(severity, base_filename, line, tm_time, message, message_len));
    }

private:
    Console* mConsole;
};

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


void Console::_Redraw()
{
    SDL_Surface* surf = SDL_CreateRGBSurface(0, 800, 400, 32, 0xf000, 0x0f00, 0x00f0, 0x000f);

    mOverlay.SetTexture(surf);

    SDL_FreeSurface(surf);
}

void Console::Exec(const std::string& cmd)
{
    cs::CritSectionLock lock(mCmdQueueCS);

    mCmdQueue.push(cmd);
}


void Console::PrintMessage(const std::string& msg)
{
    mMsgCS.Enter();

    mMessages.push_back(msg);

    _Redraw();

    mMsgCS.Leave();
}

} // namespace vh
