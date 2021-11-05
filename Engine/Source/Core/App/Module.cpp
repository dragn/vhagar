#include "Core/VhCore_PCH.hpp"

#include "Module.hpp"

namespace vh
{

void Module::Tick()
{
    if (mTickDelta == 0) return;

    switch (mState)
    {
        // -- State: Init
        //
        case ModuleState::INIT:
            switch (TickInit(mTickDelta))
            {
            case Ret::SUCCESS:
                LOG(INFO) << "Component " << GetName() << ": init finished";
                mState = ModuleState::RUN;
                break;
            case Ret::FAILURE:
                LOG(ERROR) << "Component " << GetName() << ": init failed - closing";
                mState = ModuleState::CLOSE;
                break;
            }
            break;

        // -- State: Run
        //
        case ModuleState::RUN:
            if (!mStartFrameCalled) break;
            switch (TickRun(mTickDelta))
            {
            case Ret::SUCCESS:
                LOG(INFO) << "Component " << GetName() << ": tick finished - closing";
                mState = ModuleState::CLOSE;
                break;
            case Ret::FAILURE:
                LOG(ERROR) << "Component " << GetName() << ": tick failed - closing";
                mState = ModuleState::CLOSE;
                break;
            }
            break;

        // -- State: Close
        //
        case ModuleState::CLOSE:
            switch (TickClose(mTickDelta))
            {
            case Ret::SUCCESS:
                LOG(INFO) << "Component " << GetName() << ": close finished";
                mState = ModuleState::CLOSED;
                break;
            case Ret::FAILURE:
                LOG(ERROR) << "Component " << GetName() << ": close failed";
                mState = ModuleState::CLOSED;
                break;
            }
            break;
        default:
            break;
    }
}

vh::Ret Module::TickInit(uint32_t delta)
{
    return Ret::SUCCESS;
}

vh::Ret Module::TickClose(uint32_t delta)
{
    return Ret::SUCCESS;
}

void Module::Close()
{
    if (mState != ModuleState::CLOSED && mState != ModuleState::CLOSE)
    {
        LOG(INFO) << "Component " << GetName() << ": close";
        mState = ModuleState::CLOSE;
    }
}

void Module::StartFrame_Internal(uint32_t time)
{
    mStartFrameCalled = false;

    // negative tick step - do not tick in running
    if (mTickStep < 0 && mState == ModuleState::RUN)
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
    if (IsRunning())
    {
        StartFrame();
        mStartFrameCalled = true;
    }
}


void Module::EndFrame_Internal()
{
    // if we ticked on this frame - call end frame
    if (mTickDelta > 0 && IsRunning() && mStartFrameCalled) EndFrame();
}

} // namespace vh
