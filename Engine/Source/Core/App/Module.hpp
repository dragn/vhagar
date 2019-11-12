#pragma once

#include "Core/standard.hpp"

namespace vh
{
    class ActorBehavior;

    // -- Defines a state of a module
    enum class ModuleState
    {
        INIT,       // module is initializing
        RUN,        // running
        CLOSE,      // Close() called, module is in process of releasing resources
        CLOSED      // module released all resources and can be destroyed
    };

    // -- Used to define how often should an object be updated
    enum class TickFrequency
    {
        NEVER = -1,     // Tick disabled while running
        EACH = 0,       // Tick as fast as possible
        NORMAL = 16,    // Normal tick ~60 fps
        HALF = 32,      // Half rate ~30 fps
        RARE = 50       // Rare tick
    };

    // -- This enum is used to define a result of running an action,
    //    e.g. modules init, tick and close
    enum class Ret
    {
        SUCCESS,        // result is successful, proceed to next state
        FAILURE,        // result is a failure, rollback and try to recover
        CONTINUE        // the running is in progress, call the action again on the next cycle
    };

    typedef uint32_t ModuleID;
    static const ModuleID ModuleID_Invalid = ModuleID(-1);

#define VH_MODULE(name)                                          \
friend class vh::App;                                               \
public:                                                             \
    static const char* GetNameStatic() { return #name; }            \
    virtual const char* GetName() const { return #name; }           \
    static vh::ModuleID GetIDStatic() { return name::_ID; }           \
    virtual vh::ModuleID GetID() const { return name::GetIDStatic(); }\
                                                                    \
private:                                                            \
    static vh::ModuleID _ID;                                          \

#define VH_MODULE_IMPL(name)                                     \
vh::ModuleID name::_ID = vh::ModuleID_Invalid;                          \

    class Module
    {
        friend class App;

    public:
        Module(TickFrequency tickFreq = TickFrequency::NEVER) :
            mTickStep(static_cast<int32_t>(tickFreq))
        {}

        virtual ~Module()
        {
            CHECK(mState == ModuleState::CLOSED);
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

        ModuleState GetState() const
        {
            return mState;
        }

        bool IsRunning() const
        {
            return mState == ModuleState::RUN;
        }

        void Close();

        virtual void HandleEvent(SDL_Event* event) {};

        // This template is used to override module behaviors
        template<typename BEHAVIOR_CLASS, typename... Args>
        std::unique_ptr<ActorBehavior> MakeCustomImpl(Args... args)
        {
            return nullptr;
        }

    protected:
        // this function called when module is in state INIT
        // call FinishInit inside this function to move to state RUN
        virtual Ret TickInit(uint32_t delta);

        // this tick function called when module is in state RUN
        virtual Ret TickRun(uint32_t delta) { return Ret::CONTINUE; };

        // this tick function called when module in in state CLOSE
        // call FinishClose to move to CLOSED state (and mark this module
        // ready to be destroyed)
        virtual Ret TickClose(uint32_t delta);

        // called at the start of new frame
        virtual void StartFrame() {};

        // called at the end of the frame
        virtual void EndFrame() {};

    private:
        ModuleState mState = ModuleState::INIT;
        int32_t mTickStep = 0;

        // used by App to mark module for ticks
        uint32_t mLastTick = 0;
        uint32_t mTickDelta = 0;

        void MarkForTick();

        // called by App
        void StartFrame_Internal(uint32_t time);
        void EndFrame_Internal();
    };

} // namespace vh
