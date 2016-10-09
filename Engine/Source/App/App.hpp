#pragma once

#include "Component.hpp"
#include <list>

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

class App;

// Global instance accessors
void SetApp(App* app);
App* GetApp();

class App {

public:
    App() : mState(eAppState::RUN)
    {
        SetApp(this);
    }

    void Run();

    template<typename T>
    T* AddComponent()
    {
        T* comp = new T();
        mComponents.push_back(comp);
        return comp;
    }

    template<typename T>
    static T* GetComponent()
    {
        if (GetApp() == nullptr) return nullptr;

        for (Component* comp : GetApp()->mComponents)
        {
            if (comp->GetName() == T::COMPONENT_NAME) return reinterpret_cast<T*>(comp);
        }
        return nullptr;
    }

    virtual void OnTick() {};

protected:
    virtual void HandleEvent(SDL_Event *event);

private:
    eAppState::Type mState;

    std::list<Component*> mComponents;

    void DoRun();
    void Quit();

    void HandleEvents();

    UNCOPYABLE(App);
};

} // namespace vh
