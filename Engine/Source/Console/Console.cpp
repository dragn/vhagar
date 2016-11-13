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
        std::string str(message, message_len);
        mConsole->PrintMessage(str);
    }

private:
    Console* mConsole;
};

void Console::TickInit(uint32_t delta)
{
    ConsoleCommands::RegisterAll(this);

    mOverlay = new Overlay();

    mFont = TTF_OpenFont("Assets/Fonts/Roboto-regular.ttf", FONT_SIZE);
    if (!mFont)
    {
        LOG(ERROR) << "Could not open font";
    }
    
    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;

    mSurf = SDL_CreateRGBSurface(0, App::Get<Renderer>()->GetOptions().screenWidth - 40,
        (NUM_LINES + 1) * (FONT_SIZE + LINE_SPACE), 32, rmask, gmask, bmask, amask);

    if (!mSurf)
    {
        LOG(ERROR) << SDL_GetError();
    }

    mLogSink = new ConsoleLogSink(this);
    google::AddLogSink(mLogSink);

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

    if (mSurf) SDL_FreeSurface(mSurf);

    FinishClose();
}

void Console::Register(const std::string& name, CmdHandler handler)
{
    mCommands[name] = handler;
}

void Console::_Exec(const std::string& cmd)
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


void Console::_Redraw()
{
    mOverlay->SetPos(20, 20);

    SDL_FillRect(mSurf, NULL, SDL_MapRGB(mSurf->format, 0, 0, 0));

    if (mFont)
    {
        SDL_Color fg = { 255, 255, 255, 255 };

        SDL_Surface* text;
        SDL_Rect dstrect;
        dstrect.x = 10;

        for (size_t idx = 0; idx < NUM_LINES; ++idx)
        {
            text = TTF_RenderText_Blended(mFont, mMessages[(mMsgIdx + idx) % NUM_LINES].c_str(), fg);

            dstrect.y = idx * (FONT_SIZE + LINE_SPACE);

            SDL_BlitSurface(text, NULL, mSurf, &dstrect);

            SDL_FreeSurface(text);
        }

        std::string mPrompt = "> ";
        mPrompt.append(mInput);
        mPrompt.append("|");

        text = TTF_RenderText_Blended(mFont, mPrompt.c_str(), fg);
        dstrect.y = NUM_LINES * (FONT_SIZE + LINE_SPACE);
        SDL_BlitSurface(text, NULL, mSurf, &dstrect);
        SDL_FreeSurface(text);
    }

    mOverlay->SetTexture(mSurf);
}

void Console::Exec(const std::string& cmd)
{
    cs::CritSectionLock lock(mCmdQueueCS);

    mCmdQueue.push(cmd);
}

void Console::PrintMessage(const std::string& msg)
{
    mMsgCS.Enter();

    mMessages[mMsgIdx] = msg;

    mMsgIdx++;

    if (mMsgIdx >= NUM_LINES) mMsgIdx = 0;

    if (mShowConsole) _Redraw();

    mMsgCS.Leave();
}

void Console::ToggleConsole()
{
    if (mShowConsole)
    {
        App::Get<Renderer>()->RemoveObject(mOverlay);
        mShowConsole = false;
        SDL_StopTextInput();
    }
    else
    {
        App::Get<Renderer>()->AddObject(mOverlay);
        mShowConsole = true;
        SDL_StartTextInput();
        _Redraw();
    }
}


void Console::PrintHelp()
{
    LOG(INFO) << "Commands list:";
    for (const std::pair<std::string, CmdHandler>& cmd : mCommands)
    {
        LOG(INFO) << "  " << cmd.first;
    }
}

void Console::HandleEvent(SDL_Event* event)
{
    IF_KEYDOWN(event, SDL_SCANCODE_GRAVE)
    {
        ToggleConsole();
        return;
    }

    if (mShowConsole)
    {
        IF_KEYDOWN_SYM(event, SDLK_BACKSPACE)
        {
            if (mInput.size()) mInput.pop_back();
            mCurrHistoryIdx = 0;
            _Redraw();
        }
        IF_KEYDOWN_SYM(event, SDLK_RETURN)
        {
            if (!mInput.empty())
            {
                if (mHistory[(NUM_HISTORY + mHistoryIdx - 1) % NUM_HISTORY] != mInput)
                {
                    mHistory[mHistoryIdx] = mInput;
                    mHistoryIdx = (mHistoryIdx + 1) % NUM_HISTORY;
                }
                mCurrHistoryIdx = 0;

                Exec(mInput);
                mInput.clear();
                _Redraw();
            }
        }
        if (event->type == SDL_TEXTINPUT)
        {
            if (event->text.text[0] != '`') mInput.append(event->text.text);
            mCurrHistoryIdx = 0;
            _Redraw();
        }
        IF_KEYDOWN_SYM(event, SDLK_UP)
        {
            if (mCurrHistoryIdx < NUM_HISTORY)
            {
                size_t idx = (NUM_HISTORY + mHistoryIdx - mCurrHistoryIdx - 1) % NUM_HISTORY;
                if (!mHistory[idx].empty())
                {
                    mCurrHistoryIdx++;
                    mInput = mHistory[idx];
                    _Redraw();
                }
            }
        }
        IF_KEYDOWN_SYM(event, SDLK_DOWN)
        {
            if (mCurrHistoryIdx > 1)
            {
                size_t idx = (NUM_HISTORY + mHistoryIdx - mCurrHistoryIdx + 1) % NUM_HISTORY;
                if (!mHistory[idx].empty())
                {
                    mCurrHistoryIdx--;
                    mInput = mHistory[idx];
                    _Redraw();
                }
            }
        }
    }
}

} // namespace vh
