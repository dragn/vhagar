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
        mActors.erase(
            std::remove_if(mActors.begin(), mActors.end(), [id](const Actor& actor)
            {
                return actor.mId == id;
            }),
            mActors.end()
        );
    }

    template<typename T> void DestroyActor(T*& actor)
    {
        DestroyActor(actor->mId);
    }

    const std::vector<Actor>& GetActors()
    {
        return mActors;
    }

    template<typename T>
    T* GetActorByName(const std::string& name)
    {
        for (const std::unique_ptr<Actor>& actor : mActors)
        {
            if (actor->GetName() == name)
            {
                return reinterpret_cast<T*>(actor.get());
            }
        }
        return nullptr;
    }

    Actor* CreateActor(const std::string& name);

private:
    std::vector<Actor> mActors;
    ActorFactory mActorFactory;
    Renderer* mRenderer;
    size_t mActorID = 0;

    UNCOPYABLE(World);
};

} // namespace vh
