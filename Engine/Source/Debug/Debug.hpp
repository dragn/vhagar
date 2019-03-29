#pragma once

#include "App/App.hpp"
#include "App/World.hpp"
#include "DebugVisual.hpp"
#include <SDL_ttf.h>
#include "TextOverlay.hpp"

namespace vh
{

class Debug : public Component
{
    VH_COMPONENT(Debug);

public:
    Debug();

    /* Component interface */
    virtual Ret TickInit(uint32_t delta);
    virtual Ret TickRun(uint32_t delta);
    virtual Ret TickClose(uint32_t delta);

private:
    DebugVisual mDebugVisual;
    Renderer* mRenderer;
    World* mWorld;
    TTF_Font* mFont;
    TextOverlay mStatOverlay;
};

} // namespace vh
