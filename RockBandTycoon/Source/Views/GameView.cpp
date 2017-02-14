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

    mProfile = new GameProfile(slot);
    if (!mProfile->Load())
    {
        LOG(ERROR) << "Could not load profile " << slot;
        gui->SetView(new MenuView());
    }

    CreateToolbar();

    ButtonWidget* exitBtn = new ButtonWidget("Exit");
    exitBtn->SetPos(2, 22);
    exitBtn->SetSize(24, 16);
    exitBtn->SetOnClickHandler([this, gui] ()
    {
        mProfile->Save();
        gui->SetView(new MenuView());
    });
    AddWidget(exitBtn);
}

GameView::~GameView()
{
    CHECK(mProfile);
    CHECK(mBandNameTxt);

    LOG(INFO) << "Autosave profile on GameView destruction";
    mProfile->Save();

    /* Destroy view to clear out widget bindings before releasing GameProfile */
    Destroy();

    delete mProfile;
    mProfile = nullptr;
}

void GameView::CreateToolbar()
{
    GUI2D* gui = App::Get<GUI2D>();

    Widget* topBar = new Widget();
    topBar->SetBackground("Assets/Images/bar.png");
    topBar->SetSize(480, 20);
    topBar->SetPos(0, 0);
    AddWidget(topBar);

    int32_t gridLeft = 0;
    int32_t gridStep = 60;
    int32_t gridIdx = 0;

    ImageWidget* moneyIcon = new ImageWidget("Assets/Images/money.png");
    moneyIcon->SetPos(gridLeft + gridStep * (gridIdx), 1);
    topBar->AddChild(moneyIcon);

    TextWidget* moneyTxt = new TextWidget();
    moneyTxt->SetPos(gridLeft + gridStep * (gridIdx++) + 18, 4);
    moneyTxt->SetColor(vh::Color(0x00));
    moneyTxt->SetText(mProfile->GetMoney());
    moneyTxt->Bind(mProfile->Money_OnChange);
    topBar->AddChild(moneyTxt);

    ImageWidget* skillIcon = new ImageWidget("Assets/Images/skill.png");
    skillIcon->SetPos(gridLeft + gridStep * (gridIdx++), 1);
    topBar->AddChild(skillIcon);

    ImageWidget* popIcon = new ImageWidget("Assets/Images/pop.png");
    popIcon->SetPos(gridLeft + gridStep * (gridIdx++), 1);
    topBar->AddChild(popIcon);

    ImageWidget* repIcon = new ImageWidget("Assets/Images/rep.png");
    repIcon->SetPos(gridLeft + gridStep * (gridIdx++), 1);
    topBar->AddChild(repIcon);

    mBandNameTxt = new TextWidget(mProfile->GetBandName().c_str());
    mBandNameTxt->SetPos(ePos::Right, ePos::Top, eAnchor::TopRight);
    mBandNameTxt->SetFont(gui->GetHdrFont());
    mBandNameTxt->SetColor(0x00);
    mBandNameTxt->Bind(mProfile->BandName_OnChange);

    topBar->AddChild(mBandNameTxt);
}
