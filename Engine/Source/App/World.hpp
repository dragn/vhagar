#pragma once

#include "Common.hpp"
#include "Actor/Actor.hpp"
#include "Component.hpp"
#include "Renderer/Lights.hpp"
#include <list>
#include "Actor/ActorFactory.hpp"

namespace vh {

class World : public Component
{
    COMPONENT_NAME(World);

public:
    World();

    template<typename T> T* SpawnActor() { return AddActor(new T); }
    template<typename T, typename ARG> T* SpawnActor(ARG arg) { return AddActor(new T(arg)); }
    template<typename T, typename ARG1, typename ARG2> T* SpawnActor(ARG1 arg1, ARG2 arg2) { return AddActor(new T(arg1, arg2)); }

    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    template<typename T> void DestroyActor(T*& actor)
    {
        auto iter = mActors.begin();
        while (iter != mActors.end())
        {
            if (*iter == actor)
            {
                (*iter)->OnDestroy();
                delete *iter;
                iter = mActors.erase(iter);
                actor = nullptr;
            }
            else
            {
                ++iter;
            }
        }
    }

    const std::list<Actor*>& GetActors()
    {
        return mActors;
    }

    template<typename T>
    T* GetActorByName(const std::string& name)
    {
        for (Actor* actor : mActors)
        {
            if (actor->GetName() == name)
            {
                return reinterpret_cast<T*>(actor);
            }
        }
        return nullptr;
    }

    void SpawnActorByClassName(const std::string& name)
    {
        Actor* a = mActorFactory.Create(name);
        if (a != nullptr) AddActor(a);
    }

private:
    std::list<Actor*> mActors;
    ActorFactory mActorFactory;

    template<typename T>
    T* AddActor(T* actor)
    {
        std::string name = typeid(T).name();
        name = name.substr(name.find("vh::") + 4);
        name.append("_");
        name.append(std::to_string(mActors.size()));
        return AddActor(actor, name);
    }

    template<typename T>
    T* AddActor(T* actor, const std::string& name)
    {
        mActors.push_back(actor);
        actor->SetName(name);
        actor->OnCreate();
        return actor;
    }

    UNCOPYABLE(World);
};

} // namespace vh
