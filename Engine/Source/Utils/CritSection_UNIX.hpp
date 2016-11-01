#pragma once

namespace cs
{

void Sleep(int32_t timeMs)
{
    useconds_t mksec = timeMs * 1000; // Milliseconds --> Microseconds

    usleep(mksec);
    pthread_yield();
}

class CritSectionImpl
{
public:
    CritSectionImpl()
    {
        pthread_mutexattr_t mutexAttr;
        pthread_mutexattr_init(&mutexAttr);
        pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE_NP);
        pthread_mutex_init(&_mutex, &mutexAttr);
        pthread_mutexattr_destroy(&mutexAttr);
    }
    ~CritSectionImpl()
    {
        pthread_mutex_destroy(&_mutex);
    }

    void Enter()
    {
        pthread_mutex_lock(&_mutex);
    }
    void Leave()
    {
        pthread_mutex_unlock(&_mutex);
    }
    bool TryEnter()
    {
        return pthread_mutex_trylock(&_mutex) == 0 ? true : false;
    }

private:
    pthread_mutex_t _mutex;
};

class SemaphoreImpl
{
public:
    SemaphoreImpl(uint32_t iniCount, uint32_t maxCount)
    {
        mSem = new sem_t;
        sem_init(mSem, 0, iniCount);
    }
    ~SemaphoreImpl()
    {
        sem_destroy(mSem);
        delete mSem;
    }

    void Release()
    {
        sem_post(mSem);
        return;
    }

    bool Wait(int32_t timeMs)
    {
        if (timeMs == TIME_INFINITE)
        {
            sem_wait(mSem);
            return true;
        }
        if (timeMs == 0)
        {
            return sem_trywait(mSem) == 0 ? true : false;
        }

        // Waiting for semaphore for specified time
        timespec ts;
        ts.tv_sec = timeMs / 1000;
        ts.tv_nsec = timeMs * 1000000;
        return sem_timedwait(mSem, &ts) == 0 ? true : false;
    }

private:
    sem_t* mSem;
};

static_assert(sizeof(CritSectionImpl) == CCRITICAL_SECTION_IMPL_SIZE, "");
static_assert(sizeof(SemaphoreImpl) == CSEMAPHIRE_IMPL_SIZE, "");

} // namespace cs

