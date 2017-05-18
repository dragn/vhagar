#include "Common.hpp"
#include "Debug.hpp"
#include "Renderer/Renderer.hpp"
#include "Console/ConsoleCommands.hpp"

using namespace vh;

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

vh::Debug::Debug() : Component(eTickFrequency::RARE)
{
    App::CheckRequired<Renderer>();
    App::CheckRequired<World>();
}

void vh::Debug::TickInit(uint32_t delta)
{
    Renderer* renderer = App::Get<Renderer>();

    // wait for renderer to start
    if (!renderer->IsRunning()) return;

    renderer->AddObject(&mDebugVisual);

    mWorld = App::Get<World>();

    mFont = TTF_OpenFont("Assets/Fonts/Roboto-Regular.ttf", 16);
    CHECK(mFont) << "Could not open font";

    REGISTER_COMMAND(toggle_labels);

    FinishInit();
}

void vh::Debug::TickRun(uint32_t delta)
{
    mDebugVisual.GetLabels().clear();
    for (const std::unique_ptr<Actor>& actor : mWorld->GetActors())
    {
        mDebugVisual.GetLabels().push_back(DebugLabel{ actor->GetPos(), "XXX" });
    }
}

void vh::Debug::TickClose(uint32_t delta)
{
    Renderer* renderer = App::Get<Renderer>();
    CHECK(renderer);

    renderer->RemoveObject(&mDebugVisual);

    FinishClose();
}
