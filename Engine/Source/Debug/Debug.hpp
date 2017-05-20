#pragma once

#include "App/App.hpp"
#include "App/World.hpp"
#include "DebugVisual.hpp"
#include <SDL_ttf.h>

namespace vh
{

class Debug : public Component
{
    COMPONENT_NAME(Debug);

public:
    Debug();

    /* Component interface */
    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

private:
    DebugVisual mDebugVisual;
    Renderer* mRenderer;
    World* mWorld;
    TTF_Font* mFont;
};

} // namespace vh
