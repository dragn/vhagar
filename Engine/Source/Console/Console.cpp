#include "Common.hpp"

#include "Console.hpp"
#include <sstream>
#include <iterator>
#include <fstream>

namespace vh
{

VH_COMPONENT_IMPL(Console);

void Console::TickInit(uint32_t delta)
{
    mEngine = App::Get<ConsoleEngine>();
    CHECK(mEngine);

    mRenderer = App::Get<Renderer>();
    CHECK(mRenderer);

    if (!mRenderer->IsRunning())
    {
        // Wait for renderer to init
        return;
    }

    mFont = TTF_OpenFont(VH_CONCAT(VH_XSTR(VH_ENGINE_ASSETS_DIR), "/Fonts/Roboto-Regular.ttf"), FONT_SIZE);
    if (!mFont)
    {
        LOG(WARNING) << "Could not open font. Console will not be started.";
        Close();
        return;
    }

    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;

    mSurf = SDL_CreateRGBSurface(0, mRenderer->GetOptions().screenWidth - 40,
        (NUM_LINES + 1) * (FONT_SIZE + LINE_SPACE), 32, rmask, gmask, bmask, amask);

    if (!mSurf)
    {
        LOG(ERROR) << SDL_GetError();
    }

    google::AddLogSink(this);

    std::ifstream hist(".consolehist");
    if (hist.is_open())
    {
        std::string line;
        while (std::getline(hist, line))
        {
            mHistory[mHistoryIdx] = line;
            mHistoryIdx = (mHistoryIdx + 1) % NUM_HISTORY;
        }
        hist.close();
    }

//    mOverlay.Init();

    FinishInit();
}

void Console::TickClose(uint32_t delta)
{
//    mOverlay.Destroy();

    google::RemoveLogSink(this);

    if (mSurf) SDL_FreeSurface(mSurf);

    std::ofstream hist(".consolehist");
    if (hist.is_open())
    {
        for (size_t i = 1; i <= NUM_HISTORY; ++i)
        {
            std::string& h = mHistory[(mHistoryIdx + i) % NUM_HISTORY];
            if (!h.empty())
            {
                hist << h << std::endl;
            }
        }
        hist.close();
    }

    FinishClose();
}

void Console::_Redraw()
{
    mOverlay.SetPos(20, 20);

    SDL_FillRect(mSurf, NULL, SDL_MapRGB(mSurf->format, 16, 16, 16));

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

    mOverlay.SetTexture(mSurf);
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
        mShowConsole = false;
        SDL_StopTextInput();
    }
    else
    {
        mShowConsole = true;
        SDL_StartTextInput();
        _Redraw();
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
                if (mHistory[(NUM_HISTORY + mHistoryIdx - 1) % NUM_HISTORY] != mInput
                    && mInput != "quit")
                {
                    mHistory[mHistoryIdx] = mInput;
                    mHistoryIdx = (mHistoryIdx + 1) % NUM_HISTORY;
                }
                mCurrHistoryIdx = 0;

                mEngine->Exec(mInput);
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

void Console::send(google::LogSeverity severity, const char* full_filename,
    const char* base_filename, int line,
    const struct ::tm* tm_time,
    const char* message, size_t message_len)
{
    std::string str(message, message_len);
    PrintMessage(str);
}

void Console::TickRun(uint32_t delta)
{
    if (mShowConsole)
    {
        mOverlay.Render();
    }
}

} // namespace vh
