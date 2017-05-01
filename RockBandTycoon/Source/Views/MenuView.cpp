#include "PCH.hpp"

#include "MenuView.hpp"
#include "NewGameView.hpp"
#include "Gameplay/GameProfile.hpp"
#include "GameView.hpp"
#include "Views/DialogView.hpp"

using namespace vh;
using namespace gui;

void DeleteProfile(int slot)
{
    DialogView* dlg = new DialogView("You are about to delete this band profile. Are you sure?");
    dlg->AddOption("Absolutely").Set([slot]
    {
        std::string path;
        if (GameProfile::GetProfilePath(path, slot))
        {
            int ret = remove(path.c_str());
            LOG_IF(ERROR, ret != 0) << "Could not delete profile " << slot;
            GUI2D* gui = vh::App::Get<GUI2D>();
            gui->SetView(new MenuView());
        }
    });
    dlg->AddOption("No, wait!");
    App::Get<GUI2D>()->SetModalView(dlg, eTransitionMode::NONE);
}

void NewGame(const int slot)
{
    GUI2D* gui = vh::App::Get<GUI2D>();
    CHECK(gui);

    App::Get<GUI2D>()->SetView(new NewGameView(slot));
}

MenuView::MenuView()
{
    SetBackground("Assets/Images/hire.png");

    AddSlotButton(1);
    AddSlotButton(2);
    AddSlotButton(3);

    ButtonWidget* exitBtn = new ButtonWidget("Exit");
    exitBtn->SetSize(80, 28);
    exitBtn->SetPos(ePos::Center, 200, eAnchor::CenterCenter);
    exitBtn->SetBackground(Colors::White);
    exitBtn->SetTextColor(Colors::Orange);
    exitBtn->SetBorder(Colors::Orange);
    exitBtn->OnClick.Add([] ()
    {
        vh::GetApp()->Close();
    });
    AddWidget(exitBtn);
}

bool MenuView::GetProfile(std::string& outName, int slot)
{
    GameProfile* profile = new GameProfile(slot);
    if (profile->Load())
    {
        outName = profile->GetBandName();
        return true;
    }
    return false;
}

void MenuView::AddSlotButton(int slot)
{
    ButtonWidget* slotBtn = new ButtonWidget();
    slotBtn->SetSize(140, 40);
    slotBtn->SetPos(ePos::Center, 30 + 42 * slot, eAnchor::CenterCenter);
    slotBtn->SetBackground(Colors::White);
    slotBtn->SetTextColor(Colors::Orange);
    slotBtn->SetBorder(Colors::Orange);

    std::string name;
    if (GetProfile(name, slot))
    {
        slotBtn->SetText(name.c_str());
        slotBtn->OnClick.Add([slot] ()
        {
            GUI2D* gui = vh::App::Get<GUI2D>();
            CHECK(gui);

            gui->SetView(new GameView(slot));
        });

        ButtonWidget* dltBtn = new ButtonWidget("X");
        dltBtn->SetPos(312, 30 + 42 * slot, eAnchor::CenterLeft);
        dltBtn->SetSize(20, 20);
        dltBtn->SetBackground(Colors::White);
        dltBtn->SetTextColor(Colors::Orange);
        dltBtn->SetBorder(Colors::Orange);
        dltBtn->OnClick.Add([slot] () { DeleteProfile(slot); });
        AddWidget(dltBtn);
    }
    else
    {
        slotBtn->SetText("- New Game -");
        slotBtn->OnClick.Add([slot] () { NewGame(slot); });
    }

    AddWidget(slotBtn);
}

