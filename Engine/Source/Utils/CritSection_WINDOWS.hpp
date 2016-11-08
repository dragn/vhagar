#pragma once

#ifdef CMAKE_PLATFORM_WINDOWS

/*
    Cross-platform & platform-specific декларации стандартных типов и функций
    Синхронизация
*/

namespace cs
{
void cs::Sleep(int32_t timeMs)
{
    ::Sleep((DWORD)timeMs);
}

/*
    Имплементация критической секции для Windows
*/
class CritSectionImpl
{
public:
    CritSectionImpl()
    {
        ::InitializeCriticalSection(&m_cs);
    }
    ~CritSectionImpl()
    {
        ::DeleteCriticalSection(&m_cs);
    }

    void Enter()
    {
        EnterCriticalSection(&m_cs);
    }
    void Leave()
    {
        LeaveCriticalSection(&m_cs);
    }
    bool TryEnter()
    {
        return TryEnterCriticalSection(&m_cs) ? true : false;
    }

private:
    CRITICAL_SECTION m_cs;
};

/*
    Имплементация семафора для Windows
*/
class SemaphoreImpl
{
public:
    SemaphoreImpl(uint32_t iniCount, uint32_t maxCount)
        : _sema(CreateSemaphore(NULL, (LONG)iniCount, (LONG)maxCount, NULL))
    {
    }
    ~SemaphoreImpl()
    {
        CloseHandle(_sema);
    }

    void Release()
    {
        ReleaseSemaphore(_sema, 1, NULL);
        return;
    }

    bool Wait(int32_t timeMs)
    {
        switch (WaitForSingleObject(_sema, timeMs == -1 ? INFINITE : timeMs))
        {
            case WAIT_OBJECT_0:
                return true;

            case WAIT_TIMEOUT:
                return false;
        }
        // PL_ASSERTW(FALSE, PL_T("WaitForSingleObject() failed"));
        return false;
    }

private:
    HANDLE _sema;
};

// Если этот ассерт срабатывает, надо скорректировать значение CCRITICAL_SECTION_IMPL_SIZE для текущей платформы
static_assert(sizeof(CritSectionImpl) == CCRITICAL_SECTION_IMPL_SIZE, "");
static_assert(sizeof(SemaphoreImpl) == CSEMAPHIRE_IMPL_SIZE, "");

} // namespace cs

#endif // CMAKE_PLATFORM_WINDOWS
