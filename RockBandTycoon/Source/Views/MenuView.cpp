#include "PCH.hpp"

#include "MenuView.hpp"
#include "NewGameView.hpp"

using namespace gui;

void NewGame(const int slot)
{
    GUI2D* gui = vh::App::Get<GUI2D>();
    CHECK(gui);

    gui->SetView(new NewGameView(slot));
}

MenuView::MenuView()
{
    SetBackground(vh::Color(0xa0));
    SetBackground("Assets/Images/menu_background.png");

    ButtonWidget* newGameBtn1 = new ButtonWidget("*New Game*");
    newGameBtn1->SetSize(80, 28);
    newGameBtn1->SetPos(ePos::Center, 60, eAnchor::CenterCenter);
    newGameBtn1->SetOnClickHandler([]() { NewGame(1); });
    AddWidget(newGameBtn1);

    ButtonWidget* newGameBtn2 = new ButtonWidget("*New Game*");
    newGameBtn2->SetSize(80, 28);
    newGameBtn2->SetPos(ePos::Center, 90, eAnchor::CenterCenter);
    newGameBtn2->SetOnClickHandler([]() { NewGame(2); });
    AddWidget(newGameBtn2);

    ButtonWidget* newGameBtn3 = new ButtonWidget("*New Game*");
    newGameBtn3->SetSize(80, 28);
    newGameBtn3->SetPos(ePos::Center, 120, eAnchor::CenterCenter);
    newGameBtn3->SetOnClickHandler([]() { NewGame(3); });
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

