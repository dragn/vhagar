#include "PCH.hpp"

#include "NewGameView.hpp"

using namespace gui;

NewGameView::NewGameView(int slot)
{
    SetBackground("Assets/Images/menu_background.png");

    TextWidget* enterTxt = new TextWidget("Enter band name");
    enterTxt->SetPos(ePos::Center, 80, eAnchor::BottomCenter);
    AddWidget(enterTxt);

    ButtonWidget* backBtn = new ButtonWidget("Back");
    backBtn->SetSize(40, 20);
    backBtn->SetPos(ePos::Center, 100, eAnchor::TopRight);
    AddWidget(backBtn);

    ButtonWidget* startBtn = new ButtonWidget("Start");
    startBtn->SetSize(40, 20);
    startBtn->SetPos(ePos::Center, 100, eAnchor::TopLeft);
    AddWidget(startBtn);
}
