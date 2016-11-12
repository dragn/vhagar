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

    mOverlay = new Overlay();

    mFont = TTF_OpenFont("Assets/Fonts/Roboto-regular.ttf", 20);
    if (!mFont)
    {
        LOG(ERROR) << "Could not open font";
    }

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
    if (mLogSink != nullptr)
    {
        google::RemoveLogSink(mLogSink);
        delete mLogSink;
        mLogSink = nullptr;
    }

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
    if (mFont)
    {
        SDL_Color fg = { 255, 255, 255, 255 };

        mOverlay->SetPos(20, 20);
        SDL_Surface* text = TTF_RenderText_Solid(mFont, mMessages.back().c_str(), fg);
        mOverlay->SetTexture(text);
        SDL_FreeSurface(text);
    }
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

void Console::ToggleConsole()
{
    if (mLogSink != nullptr)
    {
        google::RemoveLogSink(mLogSink);
        delete mLogSink;
        mLogSink = nullptr;

        App::GetComponent<Renderer>()->RemoveObject(mOverlay);
    }
    else
    {
        mLogSink = new ConsoleLogSink(this);
        google::AddLogSink(mLogSink);

        App::GetComponent<Renderer>()->AddObject(mOverlay);
    }
}

void Console::HandleEvent(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.scancode == SDL_SCANCODE_GRAVE)
        {
            ToggleConsole();
        }
    }
}

} // namespace vh
