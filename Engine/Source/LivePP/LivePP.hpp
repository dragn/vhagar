#pragma once

#include "App/App.hpp"
#include <windows.h>
#include "LPP_API.h"

namespace vh
{
    class LivePP : public Component
    {
        VH_COMPONENT(LivePP);

    public:
        virtual Ret TickInit(uint32_t delta) override;
        virtual Ret TickClose(uint32_t delta) override;

    private:
        HMODULE livePP;
    };

} // namespace vh
