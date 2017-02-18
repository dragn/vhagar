#include "PCH.hpp"

#include "HireView.hpp"

using namespace gui;
using namespace vh;

HireView::HireView()
{
    SetBackground("Assets/Images/hire.png");

    GUI2D* gui = App::Get<GUI2D>();

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
