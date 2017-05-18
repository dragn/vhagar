#include "Common.hpp"

#include "Component.hpp"

namespace vh
{

void Component::Tick()
{
    if (mTickDelta == 0) return;

    switch (mState)
    {
        case eCompState::INIT:
            TickInit(mTickDelta);
            break;
        case eCompState::RUN:
            TickRun(mTickDelta);
            break;
        case eCompState::CLOSE:
            TickClose(mTickDelta);
            break;
        default:
            break;
    }
}

void Component::TickInit(uint32_t delta)
{
    FinishInit();
}

void Component::TickClose(uint32_t delta)
{
    FinishClose();
}

void Component::FinishInit()
{
    LOG(INFO) << "Component " << GetName() << ": init finished";
    mState = eCompState::RUN;
}

void Component::FinishClose()
{
    LOG(INFO) << "Component " << GetName() << ": close finished";
    mState = eCompState::CLOSED;
}

void Component::Close()
{
    if (mState != eCompState::CLOSED && mState != eCompState::CLOSE)
    {
        LOG(INFO) << "Component " << GetName() << ": close";
        mState = eCompState::CLOSE;
    }
}

void Component::StartFrame_Internal()
{
    // negative tick step - do not tick in running
    if (mTickStep < 0 && mState == eCompState::RUN)
    {
        mTickDelta = 0; // no tick
        return;
    }

    // time passed since last tick
    uint32_t time = SDL_GetTicks();
    uint32_t delta = time - mLastTick;

    // check if delta is greater than tickStep (if tickStep == 0 we tick on each main loop tick)
    if (mTickStep > 0 && delta < static_cast<uint32_t>(mTickStep))
    {
        mTickDelta = 0;
        return;
    }

    // remember last tick time
    mLastTick = time;
    mTickDelta = delta;

    // call override behavior for frame start
    StartFrame();
}


void Component::EndFrame_Internal()
{
    // if we ticked on this frame - call end frame
    if (mTickDelta > 0) EndFrame();
}

} // namespace vh
