#include "PCH.hpp"

#include "HireView.hpp"
#include "Gameplay/BandMember.hpp"
#include "Components/Names.hpp"
#include "Components/Resources.hpp"
#include "Widgets/HireWidget.hpp"

using namespace gui;
using namespace vh;

void HireView::OnHire(int idx)
{
    GUI2D* gui = App::Get<GUI2D>();

    std::vector<HireItem> hires = GetHires();
    CHECK(idx >= 0 && idx < hires.size());
    HireItem& item = hires[idx];

    if (item.GetCost() <= mProfile->GetMoney())
    {
        mProfile->SetMoney(mProfile->GetMoney() - item.GetCost());
        mProfile->AddMember(BandMember(mType, item.GetName().c_str(), Item(), item.GetLooks()));
        hires.erase(hires.begin() + idx);

        switch (mType)
        {
        case eBandSlot::Guitar:
            mProfile->SetGuitarHires(hires);
            break;
        case eBandSlot::Bass:
            mProfile->SetBassHires(hires);
            break;
        case eBandSlot::Drums:
            mProfile->SetDrumHires(hires);
            break;
        }
    }

    gui->Back();
}

HireView::HireView(eBandSlot::Type type, GameProfile* profile)
    : mType(type), mProfile(profile)
{
    SetBackground("Assets/Images/hire.png");

    GUI2D* gui = App::Get<GUI2D>();

    int32_t paperW = 192;
    int32_t paperH = 80;
    int32_t paperX = 52;
    int32_t paperY = 44;

    const std::vector<HireItem>& hireItems = GetHires();

    if (hireItems.size() > 0)
    {
        HireWidget* mw = new HireWidget(0, hireItems[0], profile);
        mw->SetPos(paperX, paperY);
        mw->SetSize(192, 82);
        mw->SetBackground("Assets/Images/paper_1.png");
        mw->OnHire.Add(this, &HireView::OnHire);
        AddWidget(mw);
    }

    if (hireItems.size() > 1)
    {
        HireWidget* mw = new HireWidget(1, hireItems[1], profile);
        mw->SetPos(paperX + paperW, paperY);
        mw->SetSize(192, 82);
        mw->SetBackground("Assets/Images/paper_2.png");
        mw->OnHire.Add(this, &HireView::OnHire);
        AddWidget(mw);
    }

    if (hireItems.size() > 2)
    {
        HireWidget* mw = new HireWidget(2, hireItems[2], profile);
        mw->SetPos(paperX, paperY + paperH);
        mw->SetSize(192, 82);
        mw->SetBackground("Assets/Images/paper_3.png");
        mw->OnHire.Add(this, &HireView::OnHire);
        AddWidget(mw);
    }

    if (hireItems.size() > 3)
    {
        HireWidget* mw = new HireWidget(3, hireItems[3], profile);
        mw->SetPos(paperX + paperW, paperY + paperH);
        mw->SetSize(192, 82);
        mw->SetBackground("Assets/Images/paper_4.png");
        mw->OnHire.Add(this, &HireView::OnHire);
        AddWidget(mw);
    }

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
}

const std::vector<HireItem>& HireView::GetHires()
{
    switch (mType)
    {
    default:
    case eBandSlot::Guitar:
        return mProfile->GetGuitarHires();
    case eBandSlot::Bass:
        return mProfile->GetBassHires();
    case eBandSlot::Drums:
        return mProfile->GetDrumHires();
    }
}
