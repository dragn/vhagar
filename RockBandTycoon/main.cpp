#include "Vhagar.hpp"

#include "SDL_ttf.h"
#include <glm/gtx/transform.hpp>
#include "Renderer2D/Renderer2D.hpp"
#include "GUI2D/GUI2D.hpp"
#include "GUI2D/ButtonWidget.hpp"

using namespace vh;

class MyButton : public gui::ButtonWidget
{
public:
    virtual void OnClick(int32_t x, int32_t y) override
    {
        LOG(INFO) << "Button clicked!";
    }
};

class MenuView : public gui::View
{
public:
    MenuView()
    {
        MyButton* b = new MyButton();
        b->SetSize(80, 40);
        b->SetPos(gui::ePos::Center, gui::ePos::Center, gui::eAnchor::CenterCenter);
        AddWidget(b);
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
