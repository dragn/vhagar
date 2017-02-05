#include "PCH.hpp"

#include "GameView.hpp"
#include "MenuView.hpp"

using namespace vh;
using namespace gui;

GameView::GameView(int slot)
{
    GUI2D* gui = App::Get<GUI2D>();

    SetBackground("Assets/Images/garage.png");

    Widget* topBar = new Widget();
    topBar->SetBackground("Assets/Images/bar.png");
    topBar->SetSize(480, 20);
    topBar->SetPos(0, 0);
    AddWidget(topBar);

    mProfile = new GameProfile(slot);
    if (!mProfile->Load())
    {
        LOG(ERROR) << "Could not load profile " << slot;
        gui->SetView(new MenuView());
    }

    TextWidget* name = new TextWidget(mProfile->GetBandName());
    name->SetPos(ePos::Right, ePos::Top, eAnchor::TopRight);
    name->SetFont(gui->GetHdrFont());
    name->SetColor(0x00);
    topBar->AddChild(name);

    ButtonWidget* exitBtn = new ButtonWidget("Exit");
    exitBtn->SetPos(2, 2);
    exitBtn->SetSize(24, 16);
    exitBtn->SetOnClickHandler([gui] ()
    {
        gui->SetView(new MenuView());
    });
    topBar->AddChild(exitBtn);
}

GameView::~GameView()
{
    if (mProfile)
    {
        LOG(INFO) << "Autosave profile on GameView destruction";
        mProfile->Save();
    }
}
