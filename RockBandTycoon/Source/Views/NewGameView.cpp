#include "PCH.hpp"

#include "NewGameView.hpp"
#include "MenuView.hpp"
#include "Gameplay/GameProfile.hpp"
#include "GameView.hpp"
#include "Components/Names.hpp"
#include "Components/Resources.hpp"
#include "Widgets/BandMemberWidget.hpp"

using namespace gui;

NewGameView::NewGameView(int slot)
    : mSlot(slot)
{
    SetBackground("Assets/Images/menu_background.png");

    Resources* res = vh::App::Get<Resources>();
    std::vector<ItemRes> guitars;
    res->GetGuitars(guitars);
    if (guitars.size())
    {
        mTestGuitar = Item(guitars[0].GetName(), guitars[0].GetImg());
    }

    Widget* panel = new Widget();
    panel->SetSize(420, 210);
    panel->SetBackground(vh::Color(0x00));
    panel->SetPos(ePos::Center, ePos::Center, eAnchor::CenterCenter);
    AddWidget(panel);

    int32_t gridTop = 40;
    int32_t gridStep = 25;
    int32_t gridIdx = 0;

    int32_t leftOffset = 180;
    int32_t fieldW = 140;
    int32_t fieldH = 20;

    TextWidget* enterBandNameTxt = new TextWidget("Enter band name");
    enterBandNameTxt->SetPos(leftOffset, gridTop + (gridIdx++) * gridStep, eAnchor::BottomLeft);
    panel->AddChild(enterBandNameTxt);

    mBandNameFld = new TextFieldWidget();
    mBandNameFld->SetValue("Human Burrito Sauce");
    mBandNameFld->SetMaxSize(32);
    mBandNameFld->SetPos(leftOffset, gridTop + (gridIdx++) * gridStep, eAnchor::BottomLeft);
    mBandNameFld->SetSize(fieldW, fieldH);
    panel->AddChild(mBandNameFld);

    TextWidget* enterYourNameTxt = new TextWidget("Enter your name");
    enterYourNameTxt->SetPos(leftOffset, gridTop + (gridIdx++) * gridStep, eAnchor::BottomLeft);
    panel->AddChild(enterYourNameTxt);

    mYourNameFld = new TextFieldWidget();
    std::string name = GetRandomName();
    mYourNameFld->SetValue(name.c_str());
    mYourNameFld->SetMaxSize(32);
    mYourNameFld->SetPos(leftOffset, gridTop + (gridIdx) * gridStep, eAnchor::BottomLeft);
    mYourNameFld->SetSize(fieldW, fieldH);
    mYourNameFld->OnChange.Add([&] (const std::string& val)
    {
        BandMember mem = mBandMemberWdg->GetBandMember();
        mBandMemberWdg->SetBandMember(BandMember(mem.GetType(), val.c_str(),
            mem.GetHair().c_str(), mem.GetFace().c_str(), mem.GetBody().c_str(),
            mem.GetHands().c_str(), mem.GetLegs().c_str()));
        mem.SetItem(mTestGuitar);
    });
    panel->AddChild(mYourNameFld);

    ButtonWidget* regenNameBtn = new ButtonWidget();
    regenNameBtn->SetPos(leftOffset + fieldW + 2, gridTop + (gridIdx++) * gridStep - 1, eAnchor::BottomLeft);
    regenNameBtn->SetSize(18, 18);
    regenNameBtn->SetOnClickHandler([&] ()
    {
        mYourNameFld->SetValue(GetRandomName().c_str());
    });
    panel->AddChild(regenNameBtn);

    ButtonWidget* backBtn = new ButtonWidget("Back");
    backBtn->SetSize(40, fieldH);
    backBtn->SetPos(239, gridTop + (gridIdx) * gridStep, eAnchor::BottomRight);
    backBtn->SetOnClickHandler([] ()
    {
        GUI2D* gui = vh::App::Get<GUI2D>();
        gui->SetView(new MenuView());
    });
    panel->AddChild(backBtn);

    ButtonWidget* startBtn = new ButtonWidget("Start");
    startBtn->SetSize(40, fieldH);
    startBtn->SetPos(241, gridTop + (gridIdx++) * gridStep, eAnchor::BottomLeft);
    startBtn->SetOnClickHandler(this, &NewGameView::OnStart);
    panel->AddChild(startBtn);

    BandMember mem(eBandSlot::Guitar, mYourNameFld->GetValue(), "hair_1", "face_0",
        "guitar_body_0", "guitar_hands_0", "legs_1");
    mem.SetItem(mTestGuitar);

    mBandMemberWdg = new BandMemberWidget();
    mBandMemberWdg->SetPos(leftOffset / 2, 20, eAnchor::TopCenter);
    panel->AddChild(mBandMemberWdg);

    GenBandMember();

    ButtonWidget* genBtn = new ButtonWidget("Generate");
    genBtn->SetSize(50, fieldH);
    genBtn->SetPos(90, 160, eAnchor::TopCenter);
    genBtn->SetOnClickHandler(this, &NewGameView::GenBandMember);
    panel->AddChild(genBtn);
}

void NewGameView::OnStart()
{
    CHECK(mBandNameFld);

    GameProfile* profile = new GameProfile(mSlot, mBandNameFld->GetValue());
    profile->SetGuitarist(mBandMemberWdg->GetBandMember());
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

    Resources* bp = vh::App::Get<Resources>();
    bp->GetRandomHair(hair);
    bp->GetRandomFace(face);
    bp->GetRandomBody(body);
    bp->GetRandomHands(hands);
    bp->GetRandomLegs(legs);

    BandMember mem(eBandSlot::Guitar, mYourNameFld->GetValue(), hair.c_str(), face.c_str(),
        body.c_str(), hands.c_str(), legs.c_str());
    mem.SetItem(mTestGuitar);
    mBandMemberWdg->SetBandMember(mem);
}
