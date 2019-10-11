#pragma once

#include "Modules/standard.hpp"

namespace vh
{
    /*
          //         \\
        MMMM Network MMMM
          \\         //

        Networking support module.
    */

    class MNetwork : public Module
    {
        VH_MODULE(MNetwork);

    public:
        virtual Ret TickInit(uint32_t delta) override;
        virtual Ret TickClose(uint32_t delta) override;
    };

} // namespace vh
