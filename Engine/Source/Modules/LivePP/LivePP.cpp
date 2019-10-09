#include "Modules/VhModules_PCH.hpp"
#include "LivePP.hpp"

namespace vh
{
    static vh::CmdFlag sLivePPEnabled("-livepp");

    VH_MODULE_IMPL(LivePP);

    vh::Ret LivePP::TickInit(uint32_t delta)
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

        return Ret::SUCCESS;
    }

    Ret LivePP::TickClose(uint32_t delta)
    {
        if (GetApp()->GetCmdFlag(sLivePPEnabled))
        {
            lpp::lppDisableAllCallingModulesSync(livePP);
        }

        return Ret::SUCCESS;
    }

} // namespace vh
