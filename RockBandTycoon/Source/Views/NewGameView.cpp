#include "PCH.hpp"

#include "NewGameView.hpp"
#include "MenuView.hpp"
#include "Gameplay/GameProfile.hpp"
#include "GameView.hpp"

using namespace gui;

NewGameView::NewGameView(int slot)
    : mSlot(slot)
{
    SetBackground("Assets/Images/menu_background.png");

    TextWidget* enterTxt = new TextWidget("Enter band name");
    enterTxt->SetPos(ePos::Center, 80, eAnchor::BottomCenter);
    AddWidget(enterTxt);

    mNameFld = new TextFieldWidget();
    mNameFld->SetValue("Human Burrito Sauce");
    mNameFld->SetMaxSize(32);
    mNameFld->SetPos(ePos::Center, 82, eAnchor::TopCenter);
    mNameFld->SetSize(140, 20);
    AddWidget(mNameFld);

    ButtonWidget* backBtn = new ButtonWidget("Back");
    backBtn->SetSize(40, 20);
    backBtn->SetPos(239, 124, eAnchor::TopRight);
    backBtn->SetOnClickHandler([] ()
    {
        GUI2D* gui = vh::App::Get<GUI2D>();
        gui->SetView(new MenuView());
    });
    AddWidget(backBtn);

    ButtonWidget* startBtn = new ButtonWidget("Start");
    startBtn->SetSize(40, 20);
    startBtn->SetPos(241, 124, eAnchor::TopLeft);
    startBtn->SetOnClickHandler(this, &NewGameView::OnStart);
    AddWidget(startBtn);
}

void NewGameView::OnStart()
{
    CHECK(mNameFld);

    GameProfile* profile = new GameProfile(mSlot, mNameFld->GetValue());
    if (profile->Save())
    {
        GUI2D* gui = vh::App::Get<GUI2D>();
        CHECK(gui);

        gui->SetView(new GameView(mSlot));
    }
    else
    {
        // TODO handle error gracefully
        LOG(FATAL) << "Error saving profile";
    }
}
