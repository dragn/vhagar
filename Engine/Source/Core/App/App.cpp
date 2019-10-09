#include "Core/VhCore_PCH.hpp"

#include "App.hpp"

namespace vh
{

    static App* sApp;

    void SetApp(App* app)
    {
        if (sApp != nullptr)
        {
            LOG(FATAL) << "Second App is not allowed";
            return;
        }
        sApp = app;
    }

    App* GetApp()
    {
        return sApp;
    }

    static std::unordered_set<std::string> sCmdFlagsRegistry;

    CmdFlag::CmdFlag(const char* key)
    {
        mKey = key;
        sCmdFlagsRegistry.insert(key);
    }

    CmdFlag::~CmdFlag()
    {
        sCmdFlagsRegistry.erase(mKey);
    }

    const char* CmdFlag::GetKey() const
    {
        return mKey;
    }

    void Signal(int signal)
    {
        SDL_Event event;
        event.type = SDL_QUIT;
        SDL_PushEvent(&event);
    }

    void App::Run()
    {
#ifdef WITH_GLOG
        google::InitGoogleLogging("Vhagar");
        //google::InstallFailureSignalHandler();
        google::LogToStderr();
#endif

        std::signal(SIGINT, Signal);
#ifdef SIGQUIT
        std::signal(SIGQUIT, Signal);
#endif

        LOG(INFO) << "Starting application";
        while (mState != eAppState::CLOSED)
        {
            DoRun();

            // TODO switch to event-based main thread?
            cs::Sleep(5);
        }
    }

    void App::ParseCmd(int argc, char* argv[])
    {
        for (size_t idx = 0; idx < argc; ++idx)
        {
            std::string key(argv[idx]);
            if (sCmdFlagsRegistry.find(key) != sCmdFlagsRegistry.end())
            {
                mCmdFlags.insert(key);
            }
            else
            {
                LOG(ERROR) << "Unknown command line key: " << key;
            }
        }
    }

    void App::DoRun()
    {
        HandleEvents();

        OnTick();

        if (mState == eAppState::CLOSE && mNumModules > 0)
        {
            // -- Close modules one-by-one from the end
            int32_t modIdx;
            for (modIdx = int32_t(mNumModules) - 1; modIdx >= 0; --modIdx) if (mModules[modIdx])
            {
                const std::unique_ptr<Module>& module = mModules[modIdx];

                if (module->GetState() == ModuleState::CLOSE)
                {
                    break; // encountered closing component - wait for it to close
                }
                else if (module->GetState() == ModuleState::CLOSED)
                {
                    // component is closed - remove the reference and proceed to next one
                    LOG(INFO) << "Destroying component " << module->GetName();
                    mModules[modIdx] = nullptr;
                }
                else
                {
                    module->Close();
                    break; // stop on the first not-closed component
                }
            }
            if (modIdx == -1)
            {
                // -- all modules closed
                LOG(INFO) << "All modules closed. Stopping application";
                mState = eAppState::CLOSED;
            }
        }

        uint32_t time = SDL_GetTicks();

        // -- Dispatch StartFrame events (pass the same time for all modules,
        //    this will ensure, that modules with the same tick frequency will be ticked
        //    on the same frame.
        for (size_t modIdx = 0; modIdx < mNumModules; ++modIdx) if (mModules[modIdx])
        {
            mModules[modIdx]->StartFrame_Internal(time);
        }

        // -- Tick all modules
        for (size_t modIdx = 0; modIdx < mNumModules; ++modIdx) if (mModules[modIdx])
        {
            mModules[modIdx]->Tick();
        }

        // -- Dispatch EndFrame events
        for (size_t modIdx = 0; modIdx < mNumModules; ++modIdx) if (mModules[modIdx])
        {
            mModules[modIdx]->EndFrame_Internal();
        }
    }

    void App::Close()
    {
        mState = eAppState::CLOSE;
    }

    bool App::GetCmdFlag(const char* flag) const
    {
        return mCmdFlags.find(flag) != mCmdFlags.end();
    }

    bool App::GetCmdFlag(const CmdFlag& flag) const
    {
        return GetCmdFlag(flag.GetKey());
    }

    void App::HandleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            HandleEvent(&event);
        }
    }

    void App::HandleEvent(SDL_Event *event)
    {
        switch (event->type)
        {
        case SDL_WINDOWEVENT:
            switch (event->window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                event->type = SDL_QUIT;
                SDL_PushEvent(event);
                break;
            }
            break;
        case SDL_QUIT:
            LOG(INFO) << "Closing application";
            mState = eAppState::CLOSE;
            break;
        }

        for (size_t modIdx = 0; modIdx < mNumModules; ++modIdx) if (mModules[modIdx])
        {
            mModules[modIdx]->HandleEvent(event);
        }
    }

} // namespace vh
