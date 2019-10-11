#include "Modules/VhModules_PCH.hpp"
#include "MDebug.hpp"

#include "Modules/Console/ConsoleCommands.hpp"
#include "Modules/Renderer/MRenderer3D.hpp"

using namespace vh;

VH_MODULE_IMPL(MDebug);

DEFINE_COMMAND(toggle_labels)
{
    MDebug* debug = App::Get<MDebug>();
    if (debug == nullptr)
    {
        GetApp()->AddModule<MDebug>();
    }
    else
    {
        debug->Close();
    }
}

vh::MDebug::MDebug()
    : Module(TickFrequency::NORMAL)
    , mStatOverlay(5, 5, 200, 50)
{
}

vh::Ret vh::MDebug::TickInit(uint32_t delta)
{
    App::CheckRequired<MRenderer3D>();
    App::CheckRequired<MWorld>();

    mRenderer = App::Get<MRenderer3D>();

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

vh::Ret vh::MDebug::TickRun(uint32_t delta)
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

vh::Ret vh::MDebug::TickClose(uint32_t delta)
{
    MRenderer3D* renderer = App::Get<MRenderer3D>();
    CHECK(renderer);

    //mDebugVisual.Destroy();
    //mStatOverlay.Destroy();

    return Ret::SUCCESS;
}
