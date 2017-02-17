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

DEFINE_COMMAND(add)
{
    if (params.size() > 2)
    {
        int32_t val = stoi(params[2]);

        GameProfile* profile = nullptr;

        GUI2D* gui = App::Get<GUI2D>();
        if (gui != nullptr && gui->GetView())
        {
            if (strcmp(gui->GetView()->GetName(), "GameView") == 0)
            {
                GameView* view = reinterpret_cast<GameView*>(gui->GetView());
                profile = view->GetProfile();
            }
        }

        if (params[1] == "money")
        {
            profile->SetMoney(profile->GetMoney() + val);
        }
        else if (params[1] == "skill")
        {
            profile->SetSkill(profile->GetSkill() + val);
        }
        else if (params[1] == "pop")
        {
            profile->SetPopularity(profile->GetPopularity() + val);
        }
        else if (params[1] == "rep")
        {
            profile->SetReputation(profile->GetReputation() + val);
        }
        else if (params[1] == "quality")
        {
            profile->SetQuality(profile->GetQuality() + val);
        }
    }
    else
    {
        LOG(INFO) << "Usage: " << params[0] << " [money|skill|pop|rep|quality] <amount>";
    }
}

void DebugCommands::TickInit(uint32_t delta)
{
    REGISTER_COMMAND(set_band_name);
    REGISTER_COMMAND(add);

    FinishInit();
}

void DebugCommands::TickClose(uint32_t delta)
{
    FinishClose();
}
