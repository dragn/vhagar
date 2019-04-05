#pragma once

#include "Vhagar_PCH.hpp"
#include "Actor/Actor.hpp"
#include "Component.hpp"
#include <list>
#include "Actor/ActorFactory.hpp"
#include "Renderer/Renderer.hpp"

namespace vh
{

class World : public Component
{
    VH_COMPONENT(World);

public:
    World();

    template<typename T, typename... Args> T* SpawnActor(Args... args) { return AddActor(new T(args...)); }

    virtual Ret TickInit(uint32_t delta) override;
    virtual Ret TickRun(uint32_t delta) override;
    virtual Ret TickClose(uint32_t delta) override;

    virtual void StartFrame() override;
    virtual void EndFrame() override;

    void DestroyActor(ActorID id)
    {
        decltype(mActors)::iterator it = std::find_if(mActors.begin(), mActors.end(), [id](const std::shared_ptr<Actor>& actor)
        {
            return actor->mId == id;
        });
        if (it != mActors.end())
        {
            std::shared_ptr<Actor>& pActor = *it;
            CHECK(pActor.unique()) << "Attempt to delete locked actor";
            pActor->EndPlay();
            if (pActor != mActors.back())
            {
                std::swap(*it, mActors.back());
            }
            mActors.pop_back();
        }
    }

    // TODO Switch to visitor pattern
    const std::vector<std::shared_ptr<Actor>>& GetActors() const
    {
        return mActors;
    }

    std::weak_ptr<Actor> GetActorByName(const std::string& name) const
    {
        for (const std::shared_ptr<Actor>& actor : mActors)
        {
            if (actor->GetName() == name)
            {
                return std::weak_ptr<Actor>(actor);
            }
        }
        return std::weak_ptr<Actor>();
    }

    std::weak_ptr<Actor> CreateActor(const std::string& name);

    void ClearWorld();

private:
    std::vector<std::shared_ptr<Actor>> mActors;
    ActorFactory mActorFactory;
    Renderer* mRenderer;

    ActorID GenerateActorID() { return mActorID++; }
    ActorID mActorID = 0;

    UNCOPYABLE(World);
};

} // namespace vh
