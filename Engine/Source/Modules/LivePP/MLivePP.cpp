#include "Modules/VhModules_PCH.hpp"
#include "MLivePP.hpp"

namespace vh
{
    // static vh::CmdFlag sLivePPEnabled("-livepp");

    VH_MODULE_IMPL(MLivePP);

    vh::Ret MLivePP::TickInit(uint32_t delta)
    {
        if (GetApp()->GetCmdFlag("-livepp"))
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

    Ret MLivePP::TickClose(uint32_t delta)
    {
        if (GetApp()->GetCmdFlag("-livepp"))
        {
            lpp::lppDisableAllCallingModulesSync(livePP);
        }

        return Ret::SUCCESS;
    }

} // namespace vh
