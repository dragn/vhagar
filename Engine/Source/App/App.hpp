#pragma once

#include "Component.hpp"
#include <list>
#include <unordered_map>

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

    template<typename COMP_TYPE>
    COMP_TYPE* AddComponent()
    {
        COMP_TYPE* comp = new COMP_TYPE();
        mComponents.push_back(std::unique_ptr<Component>(comp));
        mComponentsMap[comp->GetName()] = comp;
        return comp;
    }

    template<typename COMP_TYPE, typename PARAM_TYPE>
    COMP_TYPE* AddComponent(const PARAM_TYPE& param)
    {
        COMP_TYPE* comp = new COMP_TYPE(param);
        mComponents.push_back(std::unique_ptr<Component>(comp));
        mComponentsMap[comp->GetName()] = comp;
        return comp;
    }

    template<typename T>
    static T* Get()
    {
        App* app = GetApp();
        if (app == nullptr) return nullptr;

        return reinterpret_cast<T*>(app->mComponentsMap.at(T::GetNameStatic()));
    }

    template<typename COMPONENT_TYPE>
    static void CheckRequired()
    {
        if (App::Get<COMPONENT_TYPE>() == nullptr)
        {
            LOG(FATAL) << "Missing required component: " << COMPONENT_TYPE::GetNameStatic();
        }
    }

    virtual void OnTick() {};

    virtual void Close();

protected:
    virtual void HandleEvent(SDL_Event *event);

private:
    eAppState::Type mState;

    std::list<std::unique_ptr<Component>> mComponents;
    std::unordered_map<std::string, Component*> mComponentsMap;

    void DoRun();
    void Quit();

    void HandleEvents();

    UNCOPYABLE(App);
};

} // namespace vh
