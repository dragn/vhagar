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

namespace eTickFrequency
{
enum Type : int32_t
{
    NEVER = -1,     // Tick disabled while running
    EACH = 0,       // Tick as fast as possible
    NORMAL = 16,    // Normal tick ~60 fps
    RARE = 50       // Rare tick
};
}

#define COMPONENT_NAME(name) \
public: \
    static char* GetNameStatic() { return #name; } \
    virtual char* GetName() { return #name; }

class Component
{
public:
    Component(int32_t tickStep = eTickFrequency::NEVER) :
        mState(eCompState::INIT),
        mTickStep(tickStep),
        mLastTick(0)
    {}

    virtual ~Component()
    {
        CHECK(mState == eCompState::CLOSED);
    }

    virtual char* GetName() = 0;

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
    int32_t mTickStep;
    uint32_t mLastTick;
};

} // namespace vh
