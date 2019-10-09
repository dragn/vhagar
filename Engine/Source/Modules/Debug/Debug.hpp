#pragma once

#include "Modules/Debug/DebugVisual.hpp"
#include "Modules/Debug/TextOverlay.hpp"
#include "Modules/standard.hpp"

namespace vh
{

class Debug : public Module
{
    VH_MODULE(Debug);

public:
    Debug();

    /* Component interface */
    virtual Ret TickInit(uint32_t delta);
    virtual Ret TickRun(uint32_t delta);
    virtual Ret TickClose(uint32_t delta);

private:
    DebugVisual mDebugVisual;
    Renderer* mRenderer;
    MWorld* mWorld;
    TTF_Font* mFont;
    TextOverlay mStatOverlay;
};

} // namespace vh
