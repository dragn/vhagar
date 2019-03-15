#include "Vhagar_PCH.hpp"
#include "Debug.hpp"
#include "Renderer/Renderer.hpp"
#include "Console/ConsoleCommands.hpp"

using namespace vh;

VH_COMPONENT_IMPL(Debug);

DEFINE_COMMAND(toggle_labels)
{
    Debug* debug = App::Get<Debug>();
    if (debug == nullptr)
    {
        GetApp()->AddComponent<Debug>();
    }
    else
    {
        debug->Close();
    }
}

vh::Debug::Debug()
    : Component(TickFrequency::NORMAL)
    , mStatOverlay(5, 5, 200, 50)
{
}

void vh::Debug::TickInit(uint32_t delta)
{
    App::CheckRequired<Renderer>();
    App::CheckRequired<World>();

    mRenderer = App::Get<Renderer>();

    // wait for renderer to start
    if (!mRenderer->IsRunning()) return;

    //mDebugVisual.Init();

    mWorld = App::Get<World>();

    mFont = TTF_OpenFont(VH_CONCAT(VH_XSTR(VH_ENGINE_ASSETS_DIR), "/Fonts/Roboto-Regular.ttf"), 16);
    if (mFont == nullptr)
    {
        LOG(WARNING) << "Could not open font. Debug won't load.";
        Close();
        return;
    }

    REGISTER_COMMAND(toggle_labels);

    //mStatOverlay.Init();

    FinishInit();
}

void vh::Debug::TickRun(uint32_t delta)
{
    /*
    mDebugVisual.GetLabels().clear();
    for (const std::unique_ptr<Actor>& actor : mWorld->GetActors())
    {
        mDebugVisual.GetLabels().push_back(DebugLabel{ actor->GetPos(), "XXX" });
    }

    mDebugVisual.Render(mRenderer);

    std::string fps("FPS: ");
    fps.append(std::to_string(1000.0f / delta));
    mStatOverlay.SetText(fps.c_str());
    mStatOverlay.Render();
    */
}

void vh::Debug::TickClose(uint32_t delta)
{
    Renderer* renderer = App::Get<Renderer>();
    CHECK(renderer);

    //mDebugVisual.Destroy();
    //mStatOverlay.Destroy();

    FinishClose();
}
