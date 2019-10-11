#pragma once

#include "Modules/standard.hpp"

namespace vh
{

typedef void (*CmdHandler)(const std::vector<std::string>&);

/*
    MMMM ConsoleEngine MMMM

    Allows to register and run command strings.
*/
class MConsoleEngine : public Module
{
    VH_MODULE(MConsoleEngine);

public:
    MConsoleEngine()
        : Module(TickFrequency::RARE)
    {
    }

    /* Component interface */
    virtual Ret TickInit(uint32_t delta) override;
    virtual Ret TickRun(uint32_t delta) override;

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
