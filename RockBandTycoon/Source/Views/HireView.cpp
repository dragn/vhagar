#include "PCH.hpp"

#include "HireView.hpp"
#include "Gameplay/BandMember.hpp"
#include "Components/Names.hpp"
#include "Components/Resources.hpp"
#include "Widgets/BandMemberAdWidget.hpp"

using namespace gui;
using namespace vh;

BandMember GenMember()
{
    std::string name, hair, face, body, hands, legs;
    Names* names = App::Get<Names>();
    CHECK(names);

    name = names->GetRandomMaleName();

    Resources* bp = App::Get<Resources>();
    CHECK(bp);
    bp->GetRandomHair(hair);
    bp->GetRandomFace(face);
    bp->GetRandomBody(body);
    bp->GetRandomHands(hands);
    bp->GetRandomLegs(legs);

    return BandMember(eBandSlot::Guitar, name.c_str(), hair.c_str(), face.c_str(),
        body.c_str(), hands.c_str(), legs.c_str());
}

HireView::HireView(eBandSlot::Type type, GameProfile* profile)
    : mType(type), mProfile(profile)
{
    SetBackground("Assets/Images/hire.png");

    GUI2D* gui = App::Get<GUI2D>();

    BandMember mem = GenMember();

    BandMemberAdWidget* mw1 = new BandMemberAdWidget();
    mw1->SetPos(52, 44);
    mw1->SetBackground("Assets/Images/paper_1.png");
    mw1->SetBandMember(mem);
    AddWidget(mw1);

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
