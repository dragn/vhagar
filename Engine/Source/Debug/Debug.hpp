#pragma once

#include "App/App.hpp"
#include "app/World.hpp"
#include "DebugVisual.hpp"

namespace vh
{

class Debug : public Component
{
    COMPONENT_NAME(Debug);

public:
    Debug() : Component(eTickFrequency::RARE) {};

    /* Component interface */
    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

private:
    DebugVisual mDebugVisual;
    World* mWorld;
};

} // namespace vh
