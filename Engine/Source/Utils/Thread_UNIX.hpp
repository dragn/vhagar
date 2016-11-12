namespace vh
{

class ThreadImpl
{
public:
    ThreadImpl()
        : _threadID(INVALID_THREAD_ID)
    {
    }

    ~ThreadImpl()
    {
        pthread_detach(_threadID);
        pthread_cancel(_threadID);
        _threadID = INVALID_THREAD_ID;
    }
    void Init(int stackSize, Thread* base)
    {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_attr_setstacksize(&attr, stackSize);

        int err = pthread_create(&_threadID, &attr, ThreadProc, (void*)base);
        if (err != 0)
        {
            // TODO
        }

        pthread_attr_destroy(&attr);
    }

    bool Wait(int timeMs)
    {
        int err = pthread_join(_threadID, NULL);
        if (err && err != EINVAL) // ignore non-joinable thread errors
        {
            fprintf(stderr, "join thread error: %d\n", err);
            return false;
        }
        return true;
    }

    uint32_t SetAffinity(uint32_t mask)
    {
        return 0;
    }

    bool SetThreadPriority(ThreadPrior prior)
    {
        return false;
    }

private:
    static void* ThreadProc(void* lpParameter)
    {
        Thread* pThis = (Thread*)lpParameter;
        pThis->DoExecute();

        pthread_exit(0);

        return NULL;
    }

    pthread_t _threadID;

    static const pthread_t INVALID_THREAD_ID = (pthread_t)-1;
}


} // namespace vh
