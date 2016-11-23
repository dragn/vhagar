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

void vh::Debug::TickInit(uint32_t delta)
{
    App::CheckRequired<Renderer>();
    App::CheckRequired<World>();

    Renderer* renderer = App::Get<Renderer>();
    renderer->AddObject(&mDebugVisual);

    mWorld = App::Get<World>();

    REGISTER_COMMAND(toggle_labels);

    FinishInit();
}

void vh::Debug::TickRun(uint32_t delta)
{
    mDebugVisual.GetPos().clear();
    for (const Actor* actor : mWorld->GetActors())
    {
        mDebugVisual.GetPos().push_back(actor->GetPos());
    }
}

void vh::Debug::TickClose(uint32_t delta)
{
    Renderer* renderer = App::Get<Renderer>();
    CHECK(renderer);

    renderer->RemoveObject(&mDebugVisual);

    FinishClose();
}
