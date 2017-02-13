#include "PCH.hpp"

#include "NewGameView.hpp"
#include "MenuView.hpp"
#include "Gameplay/GameProfile.hpp"
#include "GameView.hpp"
#include "Gameplay/Names.hpp"

using namespace gui;

NewGameView::NewGameView(int slot)
    : mSlot(slot)
{
    SetBackground("Assets/Images/menu_background.png");

    TextWidget* enterBandNameTxt = new TextWidget("Enter band name");
    enterBandNameTxt->SetPos(ePos::Center, 80, eAnchor::BottomCenter);
    AddWidget(enterBandNameTxt);

    mBandNameFld = new TextFieldWidget();
    mBandNameFld->SetValue("Human Burrito Sauce");
    mBandNameFld->SetMaxSize(32);
    mBandNameFld->SetPos(ePos::Center, 82, eAnchor::TopCenter);
    mBandNameFld->SetSize(140, 20);
    AddWidget(mBandNameFld);

    TextWidget* enterYourNameTxt = new TextWidget("Enter your name");
    enterYourNameTxt->SetPos(ePos::Center, 110, eAnchor::TopCenter);
    AddWidget(enterYourNameTxt);

    mYourNameFld = new TextFieldWidget();
    std::string name = GetRandomName();
    mYourNameFld->SetValue(name.c_str());
    mYourNameFld->SetMaxSize(32);
    mYourNameFld->SetPos(ePos::Center, 125, eAnchor::TopCenter);
    mYourNameFld->SetSize(140, 20);
    AddWidget(mYourNameFld);

    ButtonWidget* backBtn = new ButtonWidget("Back");
    backBtn->SetSize(40, 20);
    backBtn->SetPos(239, 149, eAnchor::TopRight);
    backBtn->SetOnClickHandler([] ()
    {
        GUI2D* gui = vh::App::Get<GUI2D>();
        gui->SetView(new MenuView());
    });
    AddWidget(backBtn);

    ButtonWidget* startBtn = new ButtonWidget("Start");
    startBtn->SetSize(40, 20);
    startBtn->SetPos(241, 149, eAnchor::TopLeft);
    startBtn->SetOnClickHandler(this, &NewGameView::OnStart);
    AddWidget(startBtn);
}

void NewGameView::OnStart()
{
    CHECK(mBandNameFld);

    GameProfile* profile = new GameProfile(mSlot, mBandNameFld->GetValue());
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

std::string NewGameView::GetRandomName()
{
    Names* names = vh::App::Get<Names>();
    if (!names)
    {
        return "Gregg Wilson";
    }

    return names->GetRandomMaleName();
}
