#pragma once

#include "Modules/standard.hpp"
#include "LPP_API.h"

namespace vh
{
    /*
          //        \\
        MMMM LivePP MMMM
          \\        //

        Live++ wrapper module.
    */
    class MLivePP : public Module
    {
        VH_MODULE(MLivePP);

    public:
        virtual Ret TickInit(uint32_t delta) override;
        virtual Ret TickClose(uint32_t delta) override;

    private:
        HMODULE livePP;
    };

} // namespace vh
