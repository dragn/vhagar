#include "PCH.hpp"

#include "MenuView.hpp"
#include "NewGameView.hpp"
#include "Gameplay/GameProfile.hpp"
#include "GameView.hpp"

using namespace gui;

void DeleteProfile(int slot)
{
    std::string path;
    if (GameProfile::GetProfilePath(path, slot))
    {
        int ret = remove(path.c_str());
        LOG_IF(ERROR, ret != 0) << "Could not delete profile " << slot;
        GUI2D* gui = vh::App::Get<GUI2D>();
        gui->SetView(new MenuView());
    }
}

void NewGame(const int slot)
{
    GUI2D* gui = vh::App::Get<GUI2D>();
    CHECK(gui);

    gui->SetView(new NewGameView(slot));
}

MenuView::MenuView()
{
    SetBackground("Assets/Images/menu_background.png");

    AddSlotButton(1);
    AddSlotButton(2);
    AddSlotButton(3);

    ButtonWidget* exitButton = new ButtonWidget("Exit");
    exitButton->SetSize(80, 28);
    exitButton->SetPos(ePos::Center, 154, eAnchor::CenterCenter);
    exitButton->OnClick.Add([] ()
    {
        vh::GetApp()->Close();
    });
    AddWidget(exitButton);
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
    slotBtn->SetSize(100, 28);
    slotBtn->SetPos(ePos::Center, 30 + 30 * slot, eAnchor::CenterCenter);

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
        dltBtn->SetPos(292, 30 + 30 * slot, eAnchor::CenterLeft);
        dltBtn->SetSize(20, 20);
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

