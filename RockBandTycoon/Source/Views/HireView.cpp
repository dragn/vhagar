#include "PCH.hpp"

#include "HireView.hpp"
#include "Gameplay/BandMember.hpp"
#include "Components/Names.hpp"
#include "Components/Resources.hpp"
#include "Widgets/HireWidget.hpp"

using namespace gui;
using namespace vh;

HireItem GenHireItem()
{
    Names* names = App::Get<Names>();
    CHECK(names);

    Resources* res = App::Get<Resources>();
    CHECK(res);

    return HireItem(names->GetRandomMaleName(), res->GetRandomLooks());
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

    HireWidget* mw1 = new HireWidget();
    mw1->SetPos(paperX, paperY);
    mw1->SetSize(192, 82);
    mw1->SetBackground("Assets/Images/paper_1.png");
    mw1->SetHireItem(GenHireItem());
    AddWidget(mw1);

    HireWidget* mw2 = new HireWidget();
    mw2->SetPos(paperX + paperW, paperY);
    mw2->SetSize(192, 82);
    mw2->SetBackground("Assets/Images/paper_2.png");
    mw2->SetHireItem(GenHireItem());
    AddWidget(mw2);

    HireWidget* mw3 = new HireWidget();
    mw3->SetPos(paperX, paperY + paperH);
    mw3->SetSize(192, 82);
    mw3->SetBackground("Assets/Images/paper_3.png");
    mw3->SetHireItem(GenHireItem());
    AddWidget(mw3);

    HireWidget* mw4 = new HireWidget();
    mw4->SetPos(paperX + paperW, paperY + paperH);
    mw4->SetSize(192, 82);
    mw4->SetBackground("Assets/Images/paper_4.png");
    mw4->SetHireItem(GenHireItem());
    AddWidget(mw4);

    ButtonWidget* backBtn = new ButtonWidget("< BACK");
    backBtn->SetSize(73, 28);
    backBtn->SetPos(ePos::Center, 222, eAnchor::TopCenter);
    backBtn->SetBackground(Color(0x00, 0x00));
    backBtn->SetBackground("Assets/Images/button.png");
    backBtn->SetTextColor(Color(0x00));
    backBtn->SetFont(gui->GetHdr2Font());
    backBtn->SetOnClickHandler([] ()
    {
        GUI2D* gui = App::Get<GUI2D>();
        gui->Back();
    });
    AddWidget(backBtn);
}
