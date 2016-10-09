#include "App.hpp"
#include "Common.hpp"
#include <csignal>

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
    std::signal(SIGINT, Signal);
    std::signal(SIGQUIT, Signal);

    LOG(INFO) << "Starting application";
    while (mState != eAppState::CLOSED) {
        DoRun();
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

    // -- Tick all components
    std::list<Component*>::iterator iter = mComponents.begin();
    while (iter != mComponents.end())
    {
        Component* comp = *iter;

        if (comp->GetState() == eCompState::CLOSED)
        {
            LOG(INFO) << "Destroying component " << comp->GetName();
            delete comp;
            iter = mComponents.erase(iter);
        }
        else
        {
            comp->Tick(SDL_GetTicks());
            iter++;
        }
    }
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
