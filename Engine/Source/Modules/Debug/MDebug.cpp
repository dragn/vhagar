#include "Modules/VhModules_PCH.hpp"
#include "MDebug.hpp"

#include "Modules/Console/ConsoleCommands.hpp"
#include "Modules/Renderer3D/MRenderer3D.hpp"
#include "Modules/ResourceSystem/ResourceTypes/RFont.hpp"

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
    WAIT_REQUIRED(MResourceSystem);
    WAIT_REQUIRED(MRenderer3D);
    WAIT_REQUIRED(MWorld);

    //mDebugVisual.Init();

    mFont = App::Get<MResourceSystem>()->GetResource<RFont>("Fonts/Roboto-Regular.ttf")->GetFont(16);
    CHECK(mFont) << "Could not open font. Debug won't load.";

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
