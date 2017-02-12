#pragma once

#include <map>
#include <queue>
#include <list>

#include "App/App.hpp"
#include "Utils/CritSection.hpp"

namespace vh
{

typedef void (*CmdHandler)(const std::vector<std::string>&);

class ConsoleEngine : public Component
{
    COMPONENT_NAME(Console);

public:
    ConsoleEngine()
        : Component(eTickFrequency::RARE)
    {
    }

    /* Component interface */
    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);

    /* Register command */
    void Register(const std::string& name, CmdHandler handler);

    /* Run command */
    void Exec(const std::string& cmd);

    /* Print help (list all registered commands) */
    void PrintHelp();

private:
    typedef std::map<std::string, CmdHandler> CmdMap;

    CmdMap mCommands;

    std::string mInput;
    cs::CritSection mCmdQueueCS;
    std::queue<std::string> mCmdQueue;

    void _Exec(const std::string& cmd);
};

} // namespace vh
