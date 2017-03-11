#pragma once

#include <map>
#include <queue>
#include <list>

#include "App/App.hpp"
#include "ConsoleCommands.hpp"
#include "Utils/CritSection.hpp"
#include "Renderable/Overlay.hpp"
#include "Renderer/Renderer.hpp"
#include "ConsoleEngine.hpp"

#include "SDL_ttf.h"

namespace vh
{

// number of lines in console output
const size_t NUM_LINES = 15;

// font size for console output
const size_t FONT_SIZE = 12;

// spacing between console output
const size_t LINE_SPACE = 6;

// number of history records
const size_t NUM_HISTORY = 10;

typedef void (*CmdHandler)(const std::vector<std::string>&);

class ConsoleLogSink;

class Console : public Component
{
    COMPONENT_NAME(Console);

public:
    Console()
        : Component(eTickFrequency::NEVER)
        , mShowConsole(false)
        , mLogSink(nullptr)
        , mFont(nullptr)
        , mMessages(NUM_LINES)
        , mMsgIdx(0)
        , mHistory(NUM_HISTORY)
        , mHistoryIdx(0)
        , mCurrHistoryIdx(0)
        , mSurf(nullptr)
        , mOverlay(nullptr)
        , mEngine(nullptr)
    {
        App::CheckRequired<ConsoleEngine>();
        App::CheckRequired<Renderer>();
    }

    /* Component interface */
    virtual void TickInit(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    virtual void HandleEvent(SDL_Event* event);

    void Register(const std::string& name, CmdHandler handler);

    void PrintMessage(const std::string& msg);
    void ToggleConsole();

    bool IsShown() const
    {
        return mShowConsole;
    }

private:
    bool mShowConsole;
    ConsoleLogSink* mLogSink;
    TTF_Font* mFont;

    std::string mInput;

    cs::CritSection mMsgCS;
    std::vector<std::string> mMessages;
    size_t mMsgIdx;

    std::vector<std::string> mHistory;
    size_t mHistoryIdx;
    size_t mCurrHistoryIdx;

    SDL_Surface* mSurf;
    Overlay* mOverlay;

    ConsoleEngine* mEngine;

    void _Redraw();
};

} // namespace vh
