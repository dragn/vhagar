#include "CritSection.hpp"

#if defined(CMAKE_PLATFORM_WINDOWS)
#include <windows.h>
#include "CritSection_WINDOWS.hpp"
#elif defined(CMAKE_PLATFORM_UNIX)
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "CritSection_UNIX.hpp"
#else
#error Undefined operation system
#endif

#include <new>

cs::CritSection::CritSection()
{
    new ((void*)impl) CritSectionImpl;
}

cs::CritSection::~CritSection()
{
    ((CritSectionImpl*)impl)->~CritSectionImpl();
}

void cs::CritSection::Enter()
{
    ((CritSectionImpl*)impl)->Enter();
}

void cs::CritSection::Leave()
{
    ((CritSectionImpl*)impl)->Leave();
}

bool cs::CritSection::TryEnter()
{
    return ((CritSectionImpl*)impl)->TryEnter();
}

cs::Semaphore::Semaphore(uint32_t iniCount, uint32_t maxCount)
{
    new ((void*)impl) SemaphoreImpl(iniCount, maxCount);
}

cs::Semaphore::~Semaphore()
{
    ((SemaphoreImpl*)impl)->~SemaphoreImpl();
}

void cs::Semaphore::Release()
{
    ((SemaphoreImpl*)impl)->Release();
}

bool cs::Semaphore::Wait(int32_t timeMs)
{
    return ((SemaphoreImpl*)impl)->Wait(timeMs);
}
