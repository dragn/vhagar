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

    template<typename T> void DestroyActor(T*& actor)
    {
        auto iter = mActors.begin();
        while (iter != mActors.end())
        {
            if (iter->get() == actor)
            {
                iter = mActors.erase(iter);
                actor = nullptr;
            }
            else
            {
                ++iter;
            }
        }
    }

    const std::list<std::unique_ptr<Actor>>& GetActors()
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

    Actor* CreateActor(const std::string& name)
    {
        std::string tmp(name);
        tmp.append("_");
        tmp.append(std::to_string(mActors.size()));

        Actor* actor = new Actor(this);
        actor->SetName(tmp);
        mActors.push_back(std::unique_ptr<Actor>(actor));
        return actor;
    }

private:
    std::list<std::unique_ptr<Actor>> mActors;
    ActorFactory mActorFactory;
    Renderer* mRenderer;

    UNCOPYABLE(World);
};

} // namespace vh
