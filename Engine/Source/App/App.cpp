#include "Common.hpp"

#include "App.hpp"
#include <csignal>
#include "Utils/CritSection.hpp"

namespace vh {

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

void Signal(int signal)
{
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}

void App::Run() {
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
    while (mState != eAppState::CLOSED) {
        DoRun();

        // TODO switch to event-based main thread?
        cs::Sleep(5);
    }
}

void App::DoRun() {
    HandleEvents();

    OnTick();

    if (mState == eAppState::CLOSE)
    {
        // -- Close components one-by-one from the end
        if (mComponents.empty())
        {
            LOG(INFO) << "All components closed. Stopping application";
            mState = eAppState::CLOSED;
        }
        else
        {
            Component* comp = mComponents.back();
            if (comp->IsRunning()) comp->Close();
        }
    }

    // -- Dispatch StartFrame events
    std::for_each(mComponents.begin(), mComponents.end(), [] (Component* comp)
    {
        if (comp->GetState() == eCompState::RUN) comp->StartFrame();
    });

    // -- Tick all components
    std::list<Component*>::iterator iter = mComponents.begin();
    while (iter != mComponents.end())
    {
        Component* comp = *iter;

        if (comp->GetState() == eCompState::CLOSED)
        {
            LOG(INFO) << "Destroying component " << comp->GetName();
            mComponentsMap.erase(mComponentsMap.find(comp->GetName()));
            delete comp;
            iter = mComponents.erase(iter);
        }
        else
        {
            comp->Tick(SDL_GetTicks());
            iter++;
        }
    }

    // -- Dispatch EndFrame events
    std::for_each(mComponents.begin(), mComponents.end(), [] (Component* comp)
    {
        if (comp->GetState() == eCompState::RUN) comp->EndFrame();
    });
}

void App::Close()
{
    mState = eAppState::CLOSE;
}

void App::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        HandleEvent(&event);
    }
}

void App::HandleEvent(SDL_Event *event) {
    switch (event->type) {
        case SDL_WINDOWEVENT:
            switch (event->window.event) {
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

    for (Component* comp : mComponents)
    {
        comp->HandleEvent(event);
    }
}

} // namespace vh
