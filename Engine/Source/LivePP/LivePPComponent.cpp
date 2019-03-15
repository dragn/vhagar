#include "Common.hpp"

#include "LivePP/LivePPComponent.hpp"

namespace vh
{
    static vh::CmdFlag sLivePPEnabled("-livepp");

    VH_COMPONENT_IMPL(LivePP);

    void LivePP::TickInit(uint32_t delta)
    {
        if (GetApp()->GetCmdFlag(sLivePPEnabled))
        {
#ifdef VH_DEPS_FOLDER
            livePP = lpp::lppLoadAndRegister(VH_XWSTR(VH_DEPS_FOLDER) L"/LivePP", "Quickstart");

            CHECK(livePP);

            // enable Live++
            lpp::lppEnableAllCallingModulesSync(livePP);

            // enable Live++'s exception handler/error recovery
            lpp::lppInstallExceptionHandler(livePP);
#endif
        }

        FinishInit();
    }

    void LivePP::TickClose(uint32_t delta)
    {
        if (GetApp()->GetCmdFlag(sLivePPEnabled))
        {
            lpp::lppDisableAllCallingModulesSync(livePP);
        }

        FinishClose();
    }

} // namespace vh
