#include "Vhagar_PCH.hpp"
#include "Debug.hpp"
#include "Modules/Renderer/Renderer.hpp"
#include "Modules/Console/ConsoleCommands.hpp"

using namespace vh;

VH_MODULE_IMPL(Debug);

DEFINE_COMMAND(toggle_labels)
{
    Debug* debug = App::Get<Debug>();
    if (debug == nullptr)
    {
        GetApp()->AddModule<Debug>();
    }
    else
    {
        debug->Close();
    }
}

vh::Debug::Debug()
    : Module(TickFrequency::NORMAL)
    , mStatOverlay(5, 5, 200, 50)
{
}

vh::Ret vh::Debug::TickInit(uint32_t delta)
{
    App::CheckRequired<Renderer>();
    App::CheckRequired<MWorld>();

    mRenderer = App::Get<Renderer>();

    // wait for renderer to start
    if (!mRenderer || !mRenderer->IsRunning())
    {
        return Ret::CONTINUE;
    }

    //mDebugVisual.Init();

    mWorld = App::Get<MWorld>();

    mFont = TTF_OpenFont(VH_CONCAT(VH_XSTR(VH_ENGINE_ASSETS_DIR), "/Fonts/Roboto-Regular.ttf"), 16);
    if (mFont == nullptr)
    {
        LOG(WARNING) << "Could not open font. Debug won't load.";
        return Ret::FAILURE;
    }

    REGISTER_COMMAND(toggle_labels);

    //mStatOverlay.Init();

    return Ret::SUCCESS;
}

vh::Ret vh::Debug::TickRun(uint32_t delta)
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

    return Ret::CONTINUE;
}

vh::Ret vh::Debug::TickClose(uint32_t delta)
{
    Renderer* renderer = App::Get<Renderer>();
    CHECK(renderer);

    //mDebugVisual.Destroy();
    //mStatOverlay.Destroy();

    return Ret::SUCCESS;
}
