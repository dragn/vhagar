#pragma once

#include <map>
#include <queue>
#include <list>

#include "App/App.hpp"
#include "ConsoleCommands.hpp"
#include "Utils/CritSection.hpp"
#include "Renderable/Overlay.hpp"
#include "Renderer/Renderer.hpp"

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
public:
    static const char* COMPONENT_NAME;

    Console()
        : Component(COMPONENT_NAME, 20)
        , mShowConsole(false)
        , mLogSink(nullptr)
        , mOverlay(nullptr)
        , mSurf(nullptr)
        , mMsgIdx(0)
        , mMessages(NUM_LINES)
        , mHistory(NUM_HISTORY)
        , mHistoryIdx(0)
        , mCurrHistoryIdx(0)
    {
        App::CheckRequired<Renderer>();
    }

    /* Component interface */
    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    virtual void HandleEvent(SDL_Event* event);

    void Register(const std::string& name, CmdHandler handler);

    void Exec(const std::string& cmd);

    void PrintMessage(const std::string& msg);
    void ToggleConsole();

    void PrintHelp();

    bool IsShown() const
    {
        return mShowConsole;
    }

private:
    bool mShowConsole;
    ConsoleLogSink* mLogSink;
    TTF_Font* mFont;

    typedef std::map<std::string, CmdHandler> CmdMap;

    CmdMap mCommands;

    std::string mInput;
    cs::CritSection mCmdQueueCS;
    std::queue<std::string> mCmdQueue;

    cs::CritSection mMsgCS;
    std::vector<std::string> mMessages;
    size_t mMsgIdx;

    std::vector<std::string> mHistory;
    size_t mHistoryIdx;
    size_t mCurrHistoryIdx;

    SDL_Surface* mSurf;
    Overlay* mOverlay;

    void _Exec(const std::string& cmd);

    void _Redraw();
};

} // namespace vh
