#include "Vhagar_PCH.hpp"

#include "Component.hpp"

namespace vh
{

void Component::Tick()
{
    if (mTickDelta == 0) return;

    switch (mState)
    {
        // -- State: Init
        //
        case CompState::INIT:
            switch (TickInit(mTickDelta))
            {
            case Ret::SUCCESS:
                LOG(INFO) << "Component " << GetName() << ": init finished";
                mState = CompState::RUN;
                break;
            case Ret::FAILURE:
                LOG(ERROR) << "Component " << GetName() << ": init failed - closing";
                mState = CompState::CLOSE;
                break;
            }
            break;

        // -- State: Run
        //
        case CompState::RUN:
            switch (TickRun(mTickDelta))
            {
            case Ret::SUCCESS:
                LOG(INFO) << "Component " << GetName() << ": tick finished - closing";
                mState = CompState::CLOSE;
                break;
            case Ret::FAILURE:
                LOG(ERROR) << "Component " << GetName() << ": tick failed - closing";
                mState = CompState::CLOSE;
                break;
            }
            break;

        // -- State: Close
        //
        case CompState::CLOSE:
            switch (TickClose(mTickDelta))
            {
            case Ret::SUCCESS:
                LOG(INFO) << "Component " << GetName() << ": close finished";
                mState = CompState::CLOSED;
                break;
            case Ret::FAILURE:
                LOG(ERROR) << "Component " << GetName() << ": close failed";
                mState = CompState::CLOSED;
                break;
            }
            break;
        default:
            break;
    }
}

vh::Ret Component::TickInit(uint32_t delta)
{
    return Ret::SUCCESS;
}

vh::Ret Component::TickClose(uint32_t delta)
{
    return Ret::SUCCESS;
}

void Component::Close()
{
    if (mState != CompState::CLOSED && mState != CompState::CLOSE)
    {
        LOG(INFO) << "Component " << GetName() << ": close";
        mState = CompState::CLOSE;
    }
}

void Component::StartFrame_Internal(uint32_t time)
{
    // negative tick step - do not tick in running
    if (mTickStep < 0 && mState == CompState::RUN)
    {
        mTickDelta = 0; // no tick
        return;
    }

    // time passed since last tick
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
