#include "Core/VhCore_PCH.hpp"

#include "MWorld.hpp"
#include "Core/App/App.hpp"

namespace vh
{

VH_MODULE_IMPL(MWorld);


MWorld::MWorld() : Module(TickFrequency::NORMAL)
{
}

vh::Ret MWorld::TickInit(uint32_t delta)
{
    return Ret::SUCCESS;
}

vh::Ret MWorld::TickRun(uint32_t delta)
{
    //LOG(INFO) << "World tick " << SDL_GetTicks();

    // Tick each actor
    for (const std::shared_ptr<Actor>& actor : mActors)
    {
        actor->Tick(delta);
    }

    return Ret::CONTINUE;
}

vh::Ret MWorld::TickClose(uint32_t delta)
{
    CHECK(mActors.empty()) << "the world is not empty";

    for (const std::shared_ptr<Actor>& actor : mActors)
    {
        actor->EndPlay();
    }
    // Destroy all actors
    mActors.clear();

    return Ret::SUCCESS;
}

std::weak_ptr<vh::Actor> MWorld::CreateActor(const std::string& name)
{
    std::string tmp(name);
    tmp.append("_");
    tmp.append(std::to_string(mActors.size()));

    std::shared_ptr<Actor> actor = std::make_shared<Actor>(GenerateActorID());

    // TODO fix root behavior initialization
    actor->mRootBehavior.Attach(actor, nullptr);

    mActors.push_back(actor);

    actor->SetName(tmp);

    LOG(INFO) << "Add actor " << tmp;

    return std::weak_ptr<Actor>(actor);
}

void MWorld::ClearWorld()
{
    for (std::shared_ptr<Actor>& actor : mActors)
    {
        CHECK(actor.unique()) << "attempt to delete locked actor";
        actor->EndPlay();
    }

    mActors.clear();
}

} // namespace vh

