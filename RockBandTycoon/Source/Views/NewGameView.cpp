#include "PCH.hpp"

#include "NewGameView.hpp"
#include "MenuView.hpp"
#include "Gameplay/GameProfile.hpp"
#include "GameView.hpp"
#include "Gameplay/Names.hpp"
#include "Widgets/BandMemberWidget.hpp"
#include "Gameplay/BodyParts.hpp"

using namespace gui;

NewGameView::NewGameView(int slot)
    : mSlot(slot)
{
    SetBackground("Assets/Images/menu_background.png");

    int32_t gridTop = 60;
    int32_t gridStep = 25;
    int32_t gridIdx = 0;

    TextWidget* enterBandNameTxt = new TextWidget("Enter band name");
    enterBandNameTxt->SetPos(ePos::Center, gridTop + (gridIdx++) * gridStep, eAnchor::BottomCenter);
    AddWidget(enterBandNameTxt);

    mBandNameFld = new TextFieldWidget();
    mBandNameFld->SetValue("Human Burrito Sauce");
    mBandNameFld->SetMaxSize(32);
    mBandNameFld->SetPos(ePos::Center, gridTop + (gridIdx++) * gridStep, eAnchor::BottomCenter);
    mBandNameFld->SetSize(140, 20);
    AddWidget(mBandNameFld);

    TextWidget* enterYourNameTxt = new TextWidget("Enter your name");
    enterYourNameTxt->SetPos(ePos::Center, gridTop + (gridIdx++) * gridStep, eAnchor::BottomCenter);
    AddWidget(enterYourNameTxt);

    mYourNameFld = new TextFieldWidget();
    std::string name = GetRandomName();
    mYourNameFld->SetValue(name.c_str());
    mYourNameFld->SetMaxSize(32);
    mYourNameFld->SetPos(ePos::Center, gridTop + (gridIdx) * gridStep, eAnchor::BottomCenter);
    mYourNameFld->SetSize(140, 20);
    AddWidget(mYourNameFld);

    ButtonWidget* regenNameBtn = new ButtonWidget();
    regenNameBtn->SetPos(312, gridTop + (gridIdx++) * gridStep - 1, eAnchor::BottomLeft);
    regenNameBtn->SetSize(18, 18);
    regenNameBtn->SetOnClickHandler([&] ()
    {
        mYourNameFld->SetValue(GetRandomName().c_str());
    });
    AddWidget(regenNameBtn);

    ButtonWidget* backBtn = new ButtonWidget("Back");
    backBtn->SetSize(40, 20);
    backBtn->SetPos(239, gridTop + (gridIdx) * gridStep, eAnchor::BottomRight);
    backBtn->SetOnClickHandler([] ()
    {
        GUI2D* gui = vh::App::Get<GUI2D>();
        gui->SetView(new MenuView());
    });
    AddWidget(backBtn);

    ButtonWidget* startBtn = new ButtonWidget("Start");
    startBtn->SetSize(40, 20);
    startBtn->SetPos(241, gridTop + (gridIdx++) * gridStep, eAnchor::BottomLeft);
    startBtn->SetOnClickHandler(this, &NewGameView::OnStart);
    AddWidget(startBtn);

    BandMember mem(mYourNameFld->GetValue(), "hair_1", "face_0",
        "guitar_body_0", "guitar_hands_0", "legs_1");

    mBandMemberWdg = new BandMemberWidget();
    mBandMemberWdg->SetPos(90, 40, eAnchor::TopCenter);
    AddWidget(mBandMemberWdg);

    GenBandMember();

    ButtonWidget* genBtn = new ButtonWidget("Generate");
    genBtn->SetSize(50, 20);
    genBtn->SetPos(90, 180, eAnchor::TopCenter);
    genBtn->SetOnClickHandler(this, &NewGameView::GenBandMember);
    AddWidget(genBtn);
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

void NewGameView::GenBandMember()
{
    std::string hair, face, body,
        hands, legs;

    BodyParts* bp = vh::App::Get<BodyParts>();
    bp->GetRandomHair(hair);
    bp->GetRandomFace(face);
    bp->GetRandomBody(body);
    bp->GetRandomHands(hands);
    bp->GetRandomLegs(legs);

    BandMember mem(mYourNameFld->GetValue(), hair.c_str(), face.c_str(),
        body.c_str(), hands.c_str(), legs.c_str());
    mBandMemberWdg->SetBandMember(mem);
}
