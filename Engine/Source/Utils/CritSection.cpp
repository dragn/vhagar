#include "CritSection.hpp"

#if defined(CMAKE_PLATFORM_WINDOWS)
#include <windows.h>
#elif defined(CMAKE_PLATFORM_UNIX)
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "CritSection_UNIX.hpp"
#else
#error Undefined operation system
#endif

#include <new>

/*
    Êîíñòðóêòîð êðèòè÷ñêîé ñåêöèè
*/
cs::CritSection::CritSection()
{
    new ((void*)impl) CritSectionImpl;
}

/*
    Äåñòðóêòîð êðèòè÷ñêîé ñåêöèè
*/
cs::CritSection::~CritSection()
{
    ((CritSectionImpl*)impl)->~CritSectionImpl();
}

/*
    Âõîä â êðèòè÷ñêóþ ñåêöèþ
*/
void cs::CritSection::Enter()
{
    ((CritSectionImpl*)impl)->Enter();
}

/*
    Âûõîä èç êðèòè÷ñêîé ñåêöèè
*/
void cs::CritSection::Leave()
{
    ((CritSectionImpl*)impl)->Leave();
}

/*
    Ïîïûòêà âõîäà â êðèòè÷åñêóþ ñåêöèþ
*/
bool cs::CritSection::TryEnter()
{
    return ((CritSectionImpl*)impl)->TryEnter();
}

/*
    Êîíñòðóêòîð ñåìàôîðà
*/
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
