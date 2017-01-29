#include "Vhagar.hpp"

#include "SDL_ttf.h"
#include <glm/gtx/transform.hpp>
#include "Renderer2D/Renderer2D.hpp"
#include "GUI2D/GUI2D.hpp"
#include "GUI2D/ButtonWidget.hpp"

using namespace vh;
using namespace gui;

class MenuView : public gui::View
{
public:
    MenuView()
    {
        gui::ButtonWidget* startGameButton = new gui::ButtonWidget("Start Game");
        startGameButton->SetSize(80, 40);
        startGameButton->SetPos(ePos::Center, 78, eAnchor::CenterCenter);
        AddWidget(startGameButton);

        gui::ButtonWidget* exitButton = new gui::ButtonWidget("Exit");
        exitButton->SetSize(80, 40);
        exitButton->SetPos(ePos::Center, 120, gui::eAnchor::CenterCenter);
        exitButton->SetOnClickHandler([]() {
            vh::GetApp()->Close();
        });
        AddWidget(exitButton);
    }
};

int main(int argc, char ** argv) {
    vh::App app;

    Renderer2DOptions ro;
    ro.screenWidth = 1600;
    ro.screenHeight = 900;
    ro.fullscreen = Renderer2DOptions::FS_Window;
    ro.monitor = Renderer2DOptions::MON_First;

    app.AddComponent<Renderer2D>(ro);
    app.AddComponent<gui::GUI2D>();

    gui::GUI2D* gui = App::Get<gui::GUI2D>();
    CHECK(gui);
    gui->SetView(new MenuView());

    app.Run();

    return 0;
}
