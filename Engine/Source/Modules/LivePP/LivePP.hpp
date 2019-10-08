#pragma once

#include "Core/App/App.hpp"
#include <windows.h>
#include "LPP_API.h"

namespace vh
{
    class LivePP : public Module
    {
        VH_MODULE(LivePP);

    public:
        virtual Ret TickInit(uint32_t delta) override;
        virtual Ret TickClose(uint32_t delta) override;

    private:
        HMODULE livePP;
    };

} // namespace vh
