#include "Vhagar_PCH.hpp"

#include "App.hpp"
#include <csignal>
#include "Utils/CritSection.hpp"

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

std::unordered_set<std::string> sCmdFlagsRegistry;

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

    if (mState == eAppState::CLOSE && mNumComp > 0)
    {
        // -- Close components one-by-one from the end
        int32_t compIdx;
        for (compIdx = int32_t(mNumComp) - 1; compIdx >= 0; --compIdx) if (mComponents[compIdx])
        {
            const std::unique_ptr<Component>& comp = mComponents[compIdx];

            if (comp->GetState() == CompState::CLOSE)
            {
                break; // encountered closing component - wait for it to close
            }
            else if (comp->GetState() == CompState::CLOSED)
            {
                // component is closed - remove the reference and proceed to next one
                LOG(INFO) << "Destroying component " << comp->GetName();
                mComponents[compIdx] = nullptr;
            }
            else
            {
                comp->Close();
                break; // stop on the first not-closed component
            }
        }
        if (compIdx == -1)
        {
            // -- all components closed
            LOG(INFO) << "All components closed. Stopping application";
            mState = eAppState::CLOSED;
        }
    }

    uint32_t time = SDL_GetTicks();

    // -- Dispatch StartFrame events (pass the same time for all components,
    //    this will ensure, that components with the same tick frequency will be ticked
    //    on the same frame.
    for (size_t compIdx = 0; compIdx < mNumComp; ++compIdx) if (mComponents[compIdx]) 
    {
        mComponents[compIdx]->StartFrame_Internal(time);
    }

    // -- Tick all components
    for (size_t compIdx = 0; compIdx < mNumComp; ++compIdx) if (mComponents[compIdx])
    {
        mComponents[compIdx]->Tick();
    }

    // -- Dispatch EndFrame events
    for (size_t compIdx = 0; compIdx < mNumComp; ++compIdx) if (mComponents[compIdx]) 
    {
        mComponents[compIdx]->EndFrame_Internal();
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

    for (size_t compIdx = 0; compIdx < mNumComp; ++compIdx) if (mComponents[compIdx]) 
    {
        mComponents[compIdx]->HandleEvent(event);
    }
}

} // namespace vh
