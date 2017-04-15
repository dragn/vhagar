#include "PCH.hpp"

#include "GameView.hpp"
#include "MenuView.hpp"
#include "Widgets/BandMemberWidget.hpp"
#include "HireView.hpp"
#include "ShopView.hpp"
#include "Views/DialogView.hpp"
#include "EventsView.hpp"

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

    for (size_t idx = 0; idx < eBandSlot::MAX; ++idx) mMemberWidgets[idx] = nullptr;

    mProfile->Guitarist_OnChange.Add(this, &GameView::HandleMemberChange);
    mProfile->Drummer_OnChange.Add(this, &GameView::HandleMemberChange);
    mProfile->Bassist_OnChange.Add(this, &GameView::HandleMemberChange);

    mProfile->Trigger_Guitarist_OnChange();
    mProfile->Trigger_Bassist_OnChange();
    mProfile->Trigger_Drummer_OnChange();

    ButtonWidget* exitBtn = new ButtonWidget("Exit");
    exitBtn->SetPos(2, 22);
    exitBtn->SetSize(24, 16);
    exitBtn->OnClick.Add([this, gui] ()
    {
        mProfile->Save();
        gui->SetView(new MenuView());
    });
    AddWidget(exitBtn);

    ButtonWidget* shopBtn = new ButtonWidget("SHOP");
    shopBtn->SetSize(73, 28);
    shopBtn->SetPos(200, 222, eAnchor::TopCenter);
    shopBtn->SetBackground(vh::Color(0x00, 0x00));
    shopBtn->SetBackground("Assets/Images/button.png");
    shopBtn->SetTextColor(vh::Color(0x00));
    shopBtn->SetFont(gui->GetHdr2Font());
    shopBtn->OnClick.Add([this] ()
    {
        GUI2D* gui = App::Get<GUI2D>();
        gui->SetModalView(new ShopView(mProfile));
    });
    AddWidget(shopBtn);

    ButtonWidget* eventsBtn = new ButtonWidget("EVENTS");
    eventsBtn->SetSize(73, 28);
    eventsBtn->SetPos(280, 222, eAnchor::TopCenter);
    eventsBtn->SetBackground(vh::Color(0x00, 0x00));
    eventsBtn->SetBackground("Assets/Images/button.png");
    eventsBtn->SetTextColor(vh::Color(0x00));
    eventsBtn->SetFont(gui->GetHdr2Font());
    eventsBtn->OnClick.Add([this] ()
    {
        GUI2D* gui = App::Get<GUI2D>();
        gui->SetModalView(new EventsView(mProfile));
    });
    AddWidget(eventsBtn);


    std::string dayStr = "DAY ";
    dayStr.append(std::to_string(mProfile->GetDay()));
    dayStr.append("\n<Message of the day>");
    DialogView* dialog = new DialogView(dayStr);
    dialog->AddOption("Proceed").Set(gui, &GUI2D::BackToMain);
    gui->SetModalView(dialog);
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
    skillIcon->SetPos(gridLeft + gridStep * (gridIdx), 1);
    topBar->AddChild(skillIcon);

    TextWidget* skillTxt = new TextWidget();
    skillTxt->SetPos(gridLeft + gridStep * (gridIdx++) + 18, 4);
    skillTxt->SetColor(vh::Color(0x00));
    skillTxt->SetText(mProfile->GetSkill());
    skillTxt->Bind(mProfile->Skill_OnChange);
    topBar->AddChild(skillTxt);

    ImageWidget* popIcon = new ImageWidget("Assets/Images/pop.png");
    popIcon->SetPos(gridLeft + gridStep * (gridIdx), 1);
    topBar->AddChild(popIcon);

    TextWidget* popTxt = new TextWidget();
    popTxt->SetPos(gridLeft + gridStep * (gridIdx++) + 18, 4);
    popTxt->SetColor(vh::Color(0x00));
    popTxt->SetText(mProfile->GetPopularity());
    popTxt->Bind(mProfile->Popularity_OnChange);
    topBar->AddChild(popTxt);

    ImageWidget* repIcon = new ImageWidget("Assets/Images/rep.png");
    repIcon->SetPos(gridLeft + gridStep * (gridIdx), 1);
    topBar->AddChild(repIcon);

    TextWidget* repTxt = new TextWidget();
    repTxt->SetPos(gridLeft + gridStep * (gridIdx++) + 18, 4);
    repTxt->SetColor(vh::Color(0x00));
    repTxt->SetText(mProfile->GetReputation());
    repTxt->Bind(mProfile->Reputation_OnChange);
    topBar->AddChild(repTxt);

    TextWidget* dayTxt = new TextWidget("Day");
    dayTxt->SetPos(gridLeft + gridStep * (gridIdx), 4);
    dayTxt->SetColor(vh::Color(0x00));
    topBar->AddChild(dayTxt);

    dayTxt = new TextWidget();
    dayTxt->SetPos(gridLeft + gridStep * (gridIdx) + 20, 4);
    dayTxt->SetColor(vh::Color(0x00));
    dayTxt->SetText(mProfile->GetDay());
    dayTxt->Bind(mProfile->Day_OnChange);
    topBar->AddChild(dayTxt);

    mBandNameTxt = new TextWidget(mProfile->GetBandName().c_str());
    mBandNameTxt->SetPos(ePos::Right, ePos::Top, eAnchor::TopRight);
    mBandNameTxt->SetFont(gui->GetHdr1Font());
    mBandNameTxt->SetColor(0x00);
    mBandNameTxt->Bind(mProfile->BandName_OnChange);

    topBar->AddChild(mBandNameTxt);
}

const char* GetHireBtnText(eBandSlot::Type type)
{
    switch (type)
    {
    case eBandSlot::Guitar: return "Hire a guitar player";
    case eBandSlot::Bass: return "Hire a bass player";
    case eBandSlot::Drums: return "Hire a drummer";
    }
    return "";
}

void SetHireBtnPos(Widget* btn, eBandSlot::Type type)
{
    switch (type)
    {
    case eBandSlot::Guitar:
        btn->SetPos(110, 190);
        break;
    case eBandSlot::Bass:
        btn->SetPos(280, 190);
        break;
    case eBandSlot::Drums:
        btn->SetPos(ePos::Center, 160, eAnchor::BottomCenter);
        break;
    }
}

void SetMemberWdgPos(Widget* wdg, eBandSlot::Type type)
{
    switch (type)
    {
    case eBandSlot::Guitar:
        wdg->SetPos(70, 82);
        break;
    case eBandSlot::Bass:
        wdg->SetPos(270, 82);
        break;
    case eBandSlot::Drums:
        wdg->SetPos(ePos::Center, 60, eAnchor::TopCenter);
        break;
    }
}

void GameView::HandleMemberChange(const BandMember& member)
{
    eBandSlot::Type type = member.GetType();

    if (mMemberWidgets[type] != nullptr)
    {
        RemoveWidget(mMemberWidgets[type]);
        delete mMemberWidgets[type];
        mMemberWidgets[type] = nullptr;
    }

    if (member.GetName().empty())
    {
        ButtonWidget* hireBtn = new ButtonWidget(GetHireBtnText(type));
        hireBtn->SetSize(80, 20);
        hireBtn->SetBackground(Colors::White);
        hireBtn->SetTextColor(Colors::Orange);
        hireBtn->SetBorder(Colors::Orange);
        SetHireBtnPos(hireBtn, type);
        hireBtn->OnClick.Add([this, type] ()
        {
            GUI2D* gui = App::Get<GUI2D>();
            gui->SetModalView(new HireView(type, mProfile));
        });
        AddWidget(hireBtn);

        mMemberWidgets[type] = hireBtn;
    }
    else
    {
        BandMemberWidget* memberWdg = new BandMemberWidget();
        SetMemberWdgPos(memberWdg, type);
        memberWdg->SetBandMember(member);
        AddWidget(memberWdg);

        mMemberWidgets[type] = memberWdg;
    }
}
