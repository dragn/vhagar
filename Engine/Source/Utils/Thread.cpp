#include "Common.hpp"
#include "Thread.hpp"

#include <cstdint>

#if defined(CMAKE_PLATFORM_WINDOWS)
#include <process.h>
#include <windows.h>
#include <TlHelp32.h>
#include "Thread_WINDOWS.hpp"
#elif defined(CMAKE_PLATFORM_UNIX)
#include "pthread.h"
#include "Thread_UNIX.hpp"
#else
#error Undefined operation system
#endif

volatile long* vh::GetThreadCounter()
{
    static volatile long threadCounter = 0;
    return &threadCounter;
}

static uint32_t g_mainThreadId = vh::GetCurrentThreadId();

bool vh::IsMainThread()
{
    return vh::GetCurrentThreadId() == g_mainThreadId;
}

vh::Thread::Thread()
    : semaExit(0, 1)
    , m_param(nullptr)
    , m_cbFunc(nullptr)
{
    new ((void*)impl) ThreadImpl;
}

vh::Thread::~Thread()
{
    Wait(-1);

    if (m_cbFunc != nullptr)
    {
        semaExit.Wait(TIME_INFINITE);
    }
}

void vh::Thread::Start(ThreadFunc cbFunc, void* param, int stackSize /*= 0*/)
{
    CHECK(m_cbFunc == nullptr) << "Second Start() is not allowed";

    m_param = param;
    m_cbFunc = cbFunc;

    ((ThreadImpl*)impl)->Init(stackSize, this);
    return;
}

bool vh::Thread::Wait(int timeMs)
{
    if (m_cbFunc == nullptr)
    {
        return true;
    }
    return ((ThreadImpl*)impl)->Wait(timeMs);
}

uint32_t vh::Thread::SetAffinity(uint32_t mask)
{
    if (m_cbFunc == nullptr)
    {
        LOG(FATAL) << "Thread does not start";
        return 0;
    }
    return ((ThreadImpl*)impl)->SetAffinity(mask);
}

bool vh::Thread::SetThreadPriority(ThreadPrior prior)
{
    if (m_cbFunc == nullptr)
    {
        LOG(FATAL) << "Thread does not start";
        return false;
    }
    return ((ThreadImpl*)impl)->SetThreadPriority(prior);
}

int vh::Thread::DoExecute()
{
    vh::InterlockedInc(vh::GetThreadCounter());

    int retCode = m_cbFunc(m_param);

    vh::InterlockedDec(vh::GetThreadCounter());

    semaExit.Release();
    return retCode;
}

#ifdef CMAKE_PLATFORM_UNIX
long vh::InterlockedInc(volatile long* a)
{
    return __sync_add_and_fetch(a, 1);
}

long vh::InterlockedSum(volatile long* a, long value)
{
    return __sync_add_and_fetch(a, value);
}

long vh::InterlockedDec(volatile long* a)
{
    return __sync_add_and_fetch(a, -1);
}

void* vh::InterlockedExcPtr(void* volatile* a, void* b)
{
    return __sync_lock_test_and_set(a, b);
}

long vh::InterlockedExc(long volatile* a, long b)
{
    return __sync_lock_test_and_set(a, b);
}

extern long vh::InterlockedCmpExc( long volatile * dest, long exc, long cmp )
{
    return __sync_val_compare_and_swap( dest, exc, cmp );
}

uint32_t vh::GetCurrentThreadId()
{
    return (uint32_t) pthread_self();
}
#endif // CMAKE_PLATFORM_UNIX

#ifdef CMAKE_PLATFORM_WINDOWS
long vh::InterlockedInc(volatile long* a)
{
    return InterlockedIncrement(a);
}

long vh::InterlockedSum(volatile long* a, long value)
{
    return InterlockedExchangeAdd(a, value);
}

long vh::InterlockedDec(volatile long* a)
{
    return InterlockedDecrement(a);
}

void* vh::InterlockedExcPtr(void* volatile* a, void* b)
{
    return InterlockedExchangePointer(a, b);
}

long vh::InterlockedExc(long volatile* a, long b)
{
    return InterlockedExchange(a, b);
}

extern long vh::InterlockedCmpExc( long volatile * dest, long exc, long cmp )
{
    return InterlockedCompareExchange( dest, exc, cmp );
}

extern void* vh::InterlockedCmpExcPtr( void* volatile * dest, void* exc, void* cmp )
{
    return InterlockedCompareExchangePointer( dest, exc, cmp );
}

uint32_t vh::GetCurrentThreadId()
{
    return ::GetCurrentThreadId();
}
#endif // CMAKE_PLATFORM_WINDOWS
