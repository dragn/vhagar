#include "PCH.hpp"
#include "DebugCommands.hpp"
#include "Views/GameView.hpp"

#include <sstream>

using namespace vh;
using namespace gui;

DEFINE_COMMAND(set_band_name)
{
    if (params.size() > 1)
    {
        std::stringstream ss;
        for (size_t idx = 1; idx < params.size(); ++idx)
        {
            ss << params[idx] << " ";
        }

        GUI2D* gui = App::Get<GUI2D>();
        if (gui != nullptr && gui->GetView())
        {
            if (strcmp(gui->GetView()->GetName(), "GameView") == 0)
            {
                GameView* view = reinterpret_cast<GameView*>(gui->GetView());
                GameProfile* profile = view->GetProfile();
                profile->SetBandName(ss.str());
            }
        }
    }
}

void DebugCommands::TickInit(uint32_t delta)
{
    REGISTER_COMMAND(set_band_name);
    FinishInit();
}

void DebugCommands::TickClose(uint32_t delta)
{
    FinishClose();
}
