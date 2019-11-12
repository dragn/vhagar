#pragma once

#include "Modules/Debug/DebugVisual.hpp"
#include "Modules/Debug/TextOverlay.hpp"
#include "Modules/standard.hpp"

namespace vh
{

class MDebug : public Module
{
    VH_MODULE(MDebug);

public:
    MDebug();

    /* Component interface */
    virtual Ret TickInit(uint32_t delta);
    virtual Ret TickRun(uint32_t delta);
    virtual Ret TickClose(uint32_t delta);

private:
    DebugVisual mDebugVisual;
    TTF_Font* mFont;
    TextOverlay mStatOverlay;
};

} // namespace vh
