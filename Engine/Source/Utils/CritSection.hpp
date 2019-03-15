#pragma once

#include "Vhagar_PCH.hpp"

/*
    Cross-platform syncronization primitives
*/

namespace cs
{
#define CCRITICAL_SECTION_IMPL_SIZE 40
#define CSEMAPHIRE_IMPL_SIZE 8

#define TIME_INFINITE (int32_t) - 1

class CritSection
{
public:
    CritSection();
    ~CritSection();

    void Enter();
    void Leave();
    bool TryEnter();

private:
    uint8_t impl[CCRITICAL_SECTION_IMPL_SIZE];
};

class CritSectionLock
{
public:
    explicit CritSectionLock(CritSection& cs)
        : m_cs(cs)
    {
        m_cs.Enter();
    }
    ~CritSectionLock()
    {
        m_cs.Leave();
    }

private:
    const CritSectionLock& operator=(const CritSectionLock& rhs)
    {
        return *this;
    }
    CritSection& m_cs;
};

/**
    Cross-platform semaphore
*/
class Semaphore
{
public:
    /// ctor
    Semaphore(uint32_t iniCount, uint32_t maxCount);
    /// dtor
    ~Semaphore();

    /// release semaphore
    void Release();
    /// entering to semaphore with timeout
    bool Wait(int32_t timeMs);

private:
    uint8_t impl[CSEMAPHIRE_IMPL_SIZE];
}; // class Semaphore

extern void Sleep(int32_t timeMs);

} // namespace cs
