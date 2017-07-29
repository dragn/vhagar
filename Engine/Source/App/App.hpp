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

const static size_t VH_MAX_COMPONENTS = 128;

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
    virtual ~App() {}

    void Run();

    template<typename COMP_TYPE>
    COMP_TYPE* AddComponent()
    {
        if (COMP_TYPE::GetIDStatic() != CompID_Invalid)
        {
            LOG(FATAL) << "Component is already registered";
            return nullptr;
        }
        mComponents[mNumComp] = std::make_unique<COMP_TYPE>();
        COMP_TYPE::_ID = CompID(mNumComp);
        mNumComp++;

        LOG(INFO) << "Registered component " << COMP_TYPE::GetNameStatic() << " with ID " << COMP_TYPE::_ID;

        return Get<COMP_TYPE>();
    }

    template<typename COMP_TYPE, typename PARAM_TYPE>
    COMP_TYPE* AddComponent(const PARAM_TYPE& param)
    {
        if (COMP_TYPE::GetIDStatic() != CompID_Invalid)
        {
            LOG(FATAL) << "Component is already registered";
            return nullptr;
        }
        mComponents[mNumComp] = std::make_unique<COMP_TYPE>(param);
        COMP_TYPE::_ID = CompID(mNumComp);
        mNumComp++;

        LOG(INFO) << "Registered component " << COMP_TYPE::GetNameStatic() << " with ID " << COMP_TYPE::_ID;

        return Get<COMP_TYPE>();
    }

    template<typename T>
    static T* Get()
    {
        App* app = GetApp();
        if (app == nullptr) return nullptr;

        if (T::_ID == CompID_Invalid || !(app->mComponents[T::_ID]))
        {
            LOG(ERROR) << "No such component: " << T::GetNameStatic();
            return nullptr;
        }
        return reinterpret_cast<T*>(app->mComponents[T::_ID].get());
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

    std::unique_ptr<Component> mComponents[VH_MAX_COMPONENTS];
    size_t mNumComp = 0;

    void DoRun();
    void Quit();

    void HandleEvents();

    UNCOPYABLE(App);
};

} // namespace vh
