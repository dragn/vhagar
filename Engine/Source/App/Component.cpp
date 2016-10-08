#include "Component.hpp"

namespace vh
{

void Component::Tick(uint32_t time)
{
    uint32_t delta = time - mLastTick;

    if (mTickStep > 0 && delta < mTickStep) return;

    mLastTick = time;

    switch (mState)
    {
        case eCompState::INIT:
            TickInit(delta);
            break;
        case eCompState::RUN:
            TickRun(delta);
            break;
        case eCompState::CLOSE:
            TickClose(delta);
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
    LOG(INFO) << "Component " << mName << ": init finished";
    mState = eCompState::RUN;
}

void Component::FinishClose()
{
    LOG(INFO) << "Component " << mName << ": close finished";
    mState = eCompState::CLOSED;
}

void Component::Close()
{
    if (mState != eCompState::CLOSED && mState != eCompState::CLOSE)
    {
        LOG(INFO) << "Component " << mName << ": close";
        mState = eCompState::CLOSE;
    }
}

} // namespace vh
