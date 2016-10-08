#pragma once

#include <string>
#include "Common.hpp"

namespace vh
{

namespace eCompState
{
enum Type
{
    INIT,
    RUN,
    CLOSE,
    CLOSED
};
}

class Component
{
public:
    Component(const std::string& name, uint32_t tickStep = 0) :
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

    uint32_t GetTickStep()
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

protected:
    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta) {};
    virtual void TickClose(uint32_t delta);

    void FinishInit();
    void FinishClose();

private:
    eCompState::Type mState;
    std::string mName;
    uint32_t mTickStep;
    uint32_t mLastTick;
};

} // namespace vh
