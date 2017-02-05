#include "PCH.hpp"

#include "GameView.hpp"
#include "MenuView.hpp"

using namespace vh;
using namespace gui;

GameView::GameView(int slot)
{
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
        GUI2D* gui = App::Get<GUI2D>();
        gui->SetView(new MenuView());
    }

    TextWidget* name = new TextWidget(mProfile->GetBandName());
    name->SetPos(ePos::Right, ePos::Top, eAnchor::TopRight);
    topBar->AddChild(name);
}

GameView::~GameView()
{
    if (mProfile)
    {
        LOG(INFO) << "Autosave profile on GameView destruction";
        mProfile->Save();
    }
}
