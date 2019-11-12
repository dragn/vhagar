#pragma once

#include "Core/App/Module.hpp"
#include "Core/standard.hpp"

namespace vh
{

    enum class eAppState
    {
        RUN,
        CLOSE,
        CLOSED
    };

    const static size_t VH_MAX_MODULES = 128;

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

        template<typename MODULE_TYPE, typename ... PARAMS>
        MODULE_TYPE* AddModule(PARAMS ... param)
        {
            if (MODULE_TYPE::GetIDStatic() != ModuleID_Invalid)
            {
                LOG(FATAL) << "Module is already registered";
                return nullptr;
            }
            mModules[mNumModules] = std::make_unique<MODULE_TYPE>(param ...);
            MODULE_TYPE::_ID = ModuleID(mNumModules);
            mNumModules++;

            LOG(INFO) << "Registered Module " << MODULE_TYPE::GetNameStatic() << " with ID " << MODULE_TYPE::_ID;

            return Get<MODULE_TYPE>();
        }

        template<typename T>
        static T* Get()
        {
            App* app = GetApp();
            if (app == nullptr) return nullptr;

            if (T::_ID == ModuleID_Invalid || !(app->mModules[T::_ID]))
            {
                LOG(ERROR) << "No such Module: " << T::GetNameStatic();
                return nullptr;
            }
            return reinterpret_cast<T*>(app->mModules[T::_ID].get());
        }

        template<typename MODULE_TYPE>
        static void CheckRequired()
        {
            if (App::Get<MODULE_TYPE>() == nullptr)
            {
                LOG(FATAL) << "Missing required Module: " << MODULE_TYPE::GetNameStatic();
            }
        }

        virtual void OnTick() {};

        virtual void Close();

        bool GetCmdFlag(const char* flag) const;
        bool GetCmdFlag(const CmdFlag& flag) const;

        template<typename FUNC>
        void ForEachModule(FUNC func)
        {
            for (std::unique_ptr<Module>& pModule : mModules)
            {
                if (pModule)
                {
                    if (!func(pModule.get()))
                    {
                        return;
                    }
                }
            }
        }

    protected:
        virtual void HandleEvent(SDL_Event *event);

    private:
        eAppState mState;

        std::unique_ptr<Module> mModules[VH_MAX_MODULES];
        size_t mNumModules = 0;

        void ParseCmd(int argc, char* argv[]);
        std::unordered_set<std::string> mCmdFlags;

        void DoRun();

        void HandleEvents();

        UNCOPYABLE(App);
    };

#define WAIT_REQUIRED(MODULE_TYPE)                                  \
    {                                                               \
        MODULE_TYPE* mod = App::Get<MODULE_TYPE>();                 \
        if (mod == nullptr || !mod->IsRunning())                    \
        {                                                           \
            return Ret::CONTINUE;                                   \
        }                                                           \
    }                                                               \

} // namespace vh
