#pragma once

#include <string>
#include <cstdint>

namespace vh
{

namespace eCompState
{
enum Type
{
    INIT,       // component is initaliazing
    RUN,        // running
    CLOSE,      // Close() called, component is in process of releasing resources
    CLOSED      // component released all resources and can be destroyed
};
}

class Component
{
public:
    Component(const std::string& name, int32_t tickStep = -1) :
        mState(eCompState::INIT),
        mName(name),
        mTickStep(tickStep),
        mLastTick(0)
    {
        LOG(INFO) << "Component " << mName << ": created";
    }

    virtual ~Component()
    {
        if (mState != eCompState::CLOSED)
        {
            LOG(ERROR) << "Component " << mName << " was not closed before destruction!";
        }
    }

    void Tick(uint32_t time);

    int32_t GetTickStep()
    {
        return mTickStep;
    }

    uint32_t GetLastTick()
    {
        return mLastTick;
    }

    eCompState::Type GetState()
    {
        return mState;
    }

    bool IsRunning()
    {
        return mState == eCompState::RUN;
    }

    void Close();

    const std::string& GetName()
    {
        return mName;
    }

    virtual void HandleEvent(SDL_Event* event) {};

protected:
    // this function called when component is in state INIT
    // call FinishInit inside this function to move to state RUN
    virtual void TickInit(uint32_t delta);

    // this tick function called when component is in state RUN
    virtual void TickRun(uint32_t delta) {};

    // this tick function called when component in in state CLOSE
    // call FinishClose to move to CLOSED state (and mark this component
    // ready to be destroyed)
    virtual void TickClose(uint32_t delta);

    void FinishInit();
    void FinishClose();

private:
    eCompState::Type mState;
    std::string mName;
    int32_t mTickStep;
    uint32_t mLastTick;
};

} // namespace vh
