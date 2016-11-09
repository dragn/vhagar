#pragma once

#include <map>
#include <queue>
#include <list>

#include "App/Component.hpp"
#include "ConsoleCommands.hpp"
#include "Utils/CritSection.hpp"
#include "Renderable/Overlay.hpp"

namespace vh
{

typedef void (*CmdHandler)(const std::vector<std::string>&);

class Console : public Component
{
public:
    static const char* COMPONENT_NAME;

    Console()
        : Component(COMPONENT_NAME, 20)
    {}

    /* Component interface */
    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    void Register(const std::string& name, CmdHandler handler);

    void Exec(const std::string& cmd);

    void PrintMessage(const std::string& msg);

private:
    typedef std::map<std::string, CmdHandler> CmdMap;

    CmdMap mCommands;

    cs::CritSection mCmdQueueCS;
    std::queue<std::string> mCmdQueue;

    cs::CritSection mMsgCS;
    std::list<std::string> mMessages;

    Overlay mOverlay;

    void _Exec(const std::string& cmd);

    void _Redraw();
};

} // namespace vh
