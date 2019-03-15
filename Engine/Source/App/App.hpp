#pragma once

#include "Component.hpp"
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace vh
{

    enum class eAppState
    {
        RUN,
        CLOSE,
        CLOSED
    };

    const static size_t VH_MAX_COMPONENTS = 128;

    class App;

    // Global instance accessors
    void SetApp(App* app);
    App* GetApp();

    class CmdFlag
    {
    public:
        CmdFlag(const char* key);
        ~CmdFlag();
        const char* GetKey() const;
    private:
        const char* mKey = nullptr;
    };

    class App
    {

    public:
        App(int argc, char* argv[]) : mState(eAppState::RUN)
        {
            SetApp(this);
            ParseCmd(argc, argv);
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

        bool GetCmdFlag(const char* flag) const;
        bool GetCmdFlag(const CmdFlag& flag) const;

    protected:
        virtual void HandleEvent(SDL_Event *event);

    private:
        eAppState mState;

        std::unique_ptr<Component> mComponents[VH_MAX_COMPONENTS];
        size_t mNumComp = 0;

        void ParseCmd(int argc, char* argv[]);
        std::unordered_set<std::string> mCmdFlags;

        void DoRun();

        void HandleEvents();

        UNCOPYABLE(App);
    };

} // namespace vh
