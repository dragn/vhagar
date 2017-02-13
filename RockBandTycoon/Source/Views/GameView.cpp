#include "PCH.hpp"

#include "GameView.hpp"
#include "MenuView.hpp"

using namespace vh;
using namespace gui;

GameView::GameView(int slot)
    : View("GameView")
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

    mBandNameTxt = new TextWidget(mProfile->GetBandName().c_str());
    mBandNameTxt->SetPos(ePos::Right, ePos::Top, eAnchor::TopRight);
    mBandNameTxt->SetFont(gui->GetHdrFont());
    mBandNameTxt->SetColor(0x00);
    mBandNameTxt->BindTo(mProfile->BandName_OnChange);

    topBar->AddChild(mBandNameTxt);

    ButtonWidget* exitBtn = new ButtonWidget("Exit");
    exitBtn->SetPos(2, 2);
    exitBtn->SetSize(24, 16);
    exitBtn->SetOnClickHandler([this, gui] ()
    {
        mProfile->Save();
        gui->SetView(new MenuView());
    });
    topBar->AddChild(exitBtn);

}

GameView::~GameView()
{
    CHECK(mProfile);
    CHECK(mBandNameTxt);

    mBandNameTxt->Unbind();

    LOG(INFO) << "Autosave profile on GameView destruction";
    mProfile->Save();

    delete mProfile;
}

void GameView::BandName_OnChange(const std::string& name)
{
    mBandNameTxt->SetText(name.c_str());
}
