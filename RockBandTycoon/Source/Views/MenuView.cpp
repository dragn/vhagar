#include "PCH.hpp"

#include "MenuView.hpp"
#include "GUI2D/ButtonWidget.hpp"

using namespace gui;

MenuView::MenuView()
{
    ButtonWidget* newGameBtn1 = new ButtonWidget("*New Game*");
    newGameBtn1->SetSize(80, 28);
    newGameBtn1->SetPos(ePos::Center, 60, eAnchor::CenterCenter);
    AddWidget(newGameBtn1);

    ButtonWidget* newGameBtn2 = new ButtonWidget("*New Game*");
    newGameBtn2->SetSize(80, 28);
    newGameBtn2->SetPos(ePos::Center, 90, eAnchor::CenterCenter);
    AddWidget(newGameBtn2);

    ButtonWidget* newGameBtn3 = new ButtonWidget("*New Game*");
    newGameBtn3->SetSize(80, 28);
    newGameBtn3->SetPos(ePos::Center, 120, eAnchor::CenterCenter);
    AddWidget(newGameBtn3);

    ButtonWidget* exitButton = new ButtonWidget("Exit");
    exitButton->SetSize(80, 28);
    exitButton->SetPos(ePos::Center, 150, eAnchor::CenterCenter);
    exitButton->SetOnClickHandler([] ()
    {
        vh::GetApp()->Close();
    });
    AddWidget(exitButton);
}

