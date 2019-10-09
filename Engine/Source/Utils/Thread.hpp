#pragma once

#include "Utils/standard.hpp"
#include "CritSection.hpp"

namespace vh
{

    /* Atomic operations */
    extern long InterlockedInc(volatile long* a);
    extern long InterlockedSum(volatile long* a, long value);
    extern long InterlockedDec(volatile long* a);
    extern long InterlockedExc(long volatile* a, long b);
    extern long InterlockedCmpExc(long volatile * dest, long exc, long cmp);
    extern void* InterlockedExcPtr(void* volatile* a, void* b);
    extern void* InterlockedCmpExcPtr(void* volatile * dest, void* exc, void* cmp);

    extern uint32_t GetCurrentThreadId();

    extern volatile long* GetThreadCounter();

#if defined(CMAKE_PLATFORM_WINDOWS)
#define CTHREAD_IMPL_SIZE 24
#elif defined(CMAKE_PLATFORM_UNIX)
#define CTHREAD_IMPL_SIZE 40
#else
#error Undefined operation system
#endif

    enum ThreadPrior
    {
        THREAD_PRIOR_LOW,
        THREAD_PRIOR_MEDIUM,
        THREAD_PRIOR_NORMAL,
        THREAD_PRIOR_HIGH,
        THREAD_PRIOR_ULTRA,
        THREAD_PRIOR_REALTIME
    };

    class ThreadImpl;
    class Thread
    {
        friend class ThreadImpl;

    public:
        typedef int(*ThreadFunc)(void* param);

        Thread();
        virtual ~Thread();

        void Start(ThreadFunc cbFunc, void* param, int stackSize = 0);

        bool Wait(int timeMs);
        uint32_t SetAffinity(uint32_t mask);
        bool SetThreadPriority(ThreadPrior prior);

    protected:
        int DoExecute();
        cs::Semaphore semaExit;
        void* m_param;
        ThreadFunc m_cbFunc;
        unsigned char impl[CTHREAD_IMPL_SIZE];
    };

    extern bool IsMainThread();

} // namespace vh
