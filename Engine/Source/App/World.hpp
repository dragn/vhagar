#pragma once

#include "Common.hpp"
#include "Actor/Actor.hpp"
#include "Component.hpp"
#include "Renderer/Light.hpp"
#include <list>

namespace vh {

class World : public Component
{
public:
    static const char* COMPONENT_NAME;

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

private:
    std::list<Actor*> mActors;

    template<typename T>
    T* AddActor(T* actor)
    {
        mActors.push_back(actor);
        actor->OnCreate();
        return actor;
    }

    UNCOPYABLE(World);
};

} // namespace vh
