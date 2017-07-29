#pragma once

#include "App/App.hpp"
#include "App/Component.hpp"

class DebugCommands : public vh::Component
{
    VH_COMPONENT(DebugCommands);

public:
    DebugCommands() : Component(vh::eTickFrequency::NEVER) {}

    virtual void TickInit(uint32_t delta);
    virtual void TickClose(uint32_t delta);

};
