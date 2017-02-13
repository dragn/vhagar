#include "PCH.hpp"

#include "SDL_ttf.h"
#include <glm/gtx/transform.hpp>
#include "Renderer2D/Renderer2D.hpp"
#include "GUI2D/GUI2D.hpp"
#include "GUI2D/ButtonWidget.hpp"
#include "Views/MenuView.hpp"
#include "Debug/DebugCommands.hpp"

#include "Gameplay/Names.hpp"

using namespace vh;
using namespace gui;

int main(int argc, char ** argv) {
    vh::App app;

    Renderer2DOptions ro;
    ro.screenWidth = 960;
    ro.screenHeight = 540;
    ro.scale = 2;
    ro.fullscreen = Renderer2DOptions::FS_Window;
    ro.monitor = Renderer2DOptions::MON_First;

    app.AddComponent<Renderer2D>(ro);

    gui::GUI2DOptions guiOpts;
    guiOpts.fontPath = "Assets/Fonts/Munro.ttf";
    guiOpts.fontSize = 10;
    guiOpts.hdrFontPath = "Assets/Fonts/Munro.ttf";
    guiOpts.hdrFontSize = 20;
    app.AddComponent<gui::GUI2D>(guiOpts);

    gui::GUI2D* gui = App::Get<gui::GUI2D>();
    CHECK(gui);
    gui->SetView(new MenuView());

    // FIXME for debug
    app.AddComponent<ConsoleEngine>();
    app.AddComponent<Network>();
    app.AddComponent<RemoteConsoleServer>();
    app.AddComponent<DebugCommands>();

    app.AddComponent<Names>();

    app.Run();

    return 0;
}
