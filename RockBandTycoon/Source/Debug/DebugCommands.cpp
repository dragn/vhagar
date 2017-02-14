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

DEFINE_COMMAND(add_money)
{
    if (params.size() > 1)
    {
        int32_t val = stoi(params[1]);

        GUI2D* gui = App::Get<GUI2D>();
        if (gui != nullptr && gui->GetView())
        {
            if (strcmp(gui->GetView()->GetName(), "GameView") == 0)
            {
                GameView* view = reinterpret_cast<GameView*>(gui->GetView());
                GameProfile* profile = view->GetProfile();
                profile->SetMoney(profile->GetMoney() + val);
            }
        }
    }
}

void DebugCommands::TickInit(uint32_t delta)
{
    REGISTER_COMMAND(set_band_name);
    REGISTER_COMMAND(add_money);

    FinishInit();
}

void DebugCommands::TickClose(uint32_t delta)
{
    FinishClose();
}
