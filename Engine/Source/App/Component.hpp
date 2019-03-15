#pragma once

#include <string>
#include <cstdint>
#include "SDL.h"

namespace vh
{

enum class CompState
{
    INIT,       // component is initaliazing
    RUN,        // running
    CLOSE,      // Close() called, component is in process of releasing resources
    CLOSED      // component released all resources and can be destroyed
};

enum class TickFrequency
{
    NEVER = -1,     // Tick disabled while running
    EACH = 0,       // Tick as fast as possible
    NORMAL = 16,    // Normal tick ~60 fps
    HALF = 32,      // Half rate ~30 fps
    RARE = 50       // Rare tick
};

typedef uint32_t CompID;
static const CompID CompID_Invalid = CompID(-1);

#define VH_COMPONENT(name)                                          \
friend class vh::App;                                               \
public:                                                             \
    static const char* GetNameStatic() { return #name; }            \
    virtual const char* GetName() const { return #name; }                 \
    static vh::CompID GetIDStatic() { return name::_ID; }           \
    virtual vh::CompID GetID() const { return name::GetIDStatic(); }\
                                                                    \
private:                                                            \
    static vh::CompID _ID;                                          \

#define VH_COMPONENT_IMPL(name)                                     \
vh::CompID name::_ID = vh::CompID_Invalid;                          \

class Component
{
    friend class App;

public:
    Component(TickFrequency tickFreq = TickFrequency::NEVER) :
        mState(CompState::INIT),
        mTickStep(static_cast<int32_t>(tickFreq)),
        mLastTick(0)
    {}

    virtual ~Component()
    {
        CHECK(mState == CompState::CLOSED);
    }

    virtual const char* GetName() const = 0;

    void Tick();

    int32_t GetTickStep() const
    {
        return mTickStep;
    }

    uint32_t GetLastTick() const
    {
        return mLastTick;
    }

    CompState GetState() const
    {
        return mState;
    }

    bool IsRunning() const
    {
        return mState == CompState::RUN;
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

    // called at the start of new frame
    virtual void StartFrame() {};

    // called at the end of the frame
    virtual void EndFrame() {};

    void FinishInit();
    void FinishClose();

private:
    CompState mState;
    int32_t mTickStep;

    // used by App to mark component for ticks
    uint32_t mLastTick;
    uint32_t mTickDelta;
    void MarkForTick();

    // called by App
    void StartFrame_Internal(uint32_t time);
    void EndFrame_Internal();
};

} // namespace vh
