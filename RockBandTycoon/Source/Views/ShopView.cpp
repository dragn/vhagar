#include "PCH.hpp"

#include "ShopView.hpp"
#include "Components/Resources.hpp"
#include "Widgets/ShopItemWidget.hpp"

using namespace vh;
using namespace gui;

ShopView::ShopView(GameProfile* profile)
    : mProfile(profile)
    , mType(eBandSlot::MAX)
    , mList(nullptr)
{
    GUI2D* gui = App::Get<GUI2D>();

    SetBackground("Assets/Images/shop.png");

    mBtn[eBandSlot::Guitar] = new ButtonWidget("Guitar player");
    mBtn[eBandSlot::Guitar]->SetBackground(Colors::White);
    mBtn[eBandSlot::Guitar]->SetTextColor(Colors::Orange);
    mBtn[eBandSlot::Guitar]->SetBorder(Colors::Orange);
    mBtn[eBandSlot::Guitar]->SetSize(63, 20);
    mBtn[eBandSlot::Guitar]->SetPos(112, 6);
    mBtn[eBandSlot::Guitar]->OnClick.Add([this] () { SetType(eBandSlot::Guitar); });
    AddWidget(mBtn[eBandSlot::Guitar]);

    mBtn[eBandSlot::Bass] = new ButtonWidget("Bass player");
    mBtn[eBandSlot::Bass]->SetBackground(Colors::White);
    mBtn[eBandSlot::Bass]->SetTextColor(Colors::Orange);
    mBtn[eBandSlot::Bass]->SetBorder(Colors::Orange);
    mBtn[eBandSlot::Bass]->SetSize(56, 20);
    mBtn[eBandSlot::Bass]->SetPos(177, 6);
    mBtn[eBandSlot::Bass]->OnClick.Add([this] () { SetType(eBandSlot::Bass); });
    AddWidget(mBtn[eBandSlot::Bass]);

    mBtn[eBandSlot::Drums] = new ButtonWidget("Drummer");
    mBtn[eBandSlot::Drums]->SetBackground(Colors::White);
    mBtn[eBandSlot::Drums]->SetTextColor(Colors::Orange);
    mBtn[eBandSlot::Drums]->SetBorder(Colors::Orange);
    mBtn[eBandSlot::Drums]->SetSize(49, 20);
    mBtn[eBandSlot::Drums]->SetPos(236, 6);
    mBtn[eBandSlot::Drums]->OnClick.Add([this] () { SetType(eBandSlot::Drums); });
    AddWidget(mBtn[eBandSlot::Drums]);

    ButtonWidget* backBtn = new ButtonWidget("< BACK");
    backBtn->SetSize(73, 28);
    backBtn->SetPos(ePos::Center, 222, eAnchor::TopCenter);
    backBtn->SetBackground(Color(0x00, 0x00));
    backBtn->SetBackground("Assets/Images/button.png");
    backBtn->SetTextColor(Color(0x00));
    backBtn->SetFont(gui->GetHdr2Font());
    backBtn->OnClick.Add([] ()
    {
        GUI2D* gui = App::Get<GUI2D>();
        gui->Back();
    });
    AddWidget(backBtn);

    mMemberWidget = new BandMemberWidget();
    mMemberWidget->SetPos(20, 80);
    AddWidget(mMemberWidget);

    SetType(eBandSlot::Guitar);
}

void ShopView::SetType(eBandSlot::Type type)
{
    if (type == mType) return;

    if (mType != eBandSlot::MAX)
    {
        mBtn[mType]->SetBackground(Colors::White);
        mBtn[mType]->SetTextColor(Colors::Orange);
    }

    mBtn[type]->SetBackground(Colors::Orange);
    mBtn[type]->SetTextColor(Colors::White);

    mType = type;

    Resources* res = App::Get<Resources>();

    mItems.clear();
    switch (type)
    {
    case eBandSlot::Guitar:
        res->GetGuitars(mItems);
        break;
    case eBandSlot::Bass:
        res->GetBassGuitars(mItems);
        break;
    case eBandSlot::Drums:
        res->GetDrums(mItems);
        break;
    }

    mMemberWidget->SetBandMember(BandMember(mType, "",
        Item(), mProfile->GetBandMember(mType).GetLooks()));

    DrawItems();
}

void ShopView::DrawItems()
{
    if (mList != nullptr)
    {
        RemoveWidget(mList);
        delete mList;
        mList = nullptr;
    }

    mList = new gui::Widget();
    mList->SetSize(347, 240);
    mList->SetPos(112, 30);

    for (size_t idx = 0; idx < mItems.size(); ++idx)
    {
        ShopItemWidget* wdg = new ShopItemWidget(mItems[idx]);
        wdg->SetPos(0, 68 * idx);
        wdg->OnFocus.Add([=] ()
        {
            Item item(wdg->GetItem().GetName(), wdg->GetItem().GetImg());
            mMemberWidget->SetBandMember(BandMember(mType, "",
                item, mProfile->GetBandMember(mType).GetLooks()));
        });
        mList->AddChild(wdg);
    }

    AddWidget(mList);
}
