#pragma once

#include "Component.hpp"

namespace vh {

namespace eAppState
{
enum Type
{
    RUN,
    CLOSE,
    CLOSED
};
}

class App {

public:
    App() : mState(eAppState::RUN) {}

    void Run();

    template<typename T>
    T* SpawnComponent(const std::string& name)
    {
        T* comp = new T(name);
        mComponents.push_back(comp);
        return comp;
    }

    template<typename T>
    T* GetComponent(const std::string& name)
    {
        for (Component* comp : mComponents)
        {
            if (comp->GetName() == name) return reinterpret_cast<T*>(comp);
        }
        return nullptr;
    }

    void DestroyComponent(const std::string& name);

    virtual void OnTick() {};

private:
    eAppState::Type mState;

    std::vector<Component*> mComponents;

    void DoRun();
    void Quit();

    void HandleEvents();
    void HandleEvent(SDL_Event *event);

    UNCOPYABLE(App);
};

} // namespace vh
