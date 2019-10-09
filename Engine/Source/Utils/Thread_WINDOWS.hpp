namespace vh
{

    class ThreadImpl
    {
    public:
        ThreadImpl()
            : m_threadHandle(INVALID_HANDLE_VALUE)
            , m_threadId(0)
        {
        }

        ~ThreadImpl()
        {
        }
        void Init(int stackSize, Thread* base)
        {
            m_threadHandle = (HANDLE)_beginthreadex(NULL, (unsigned)stackSize, ThreadProc, base, 0, &m_threadId);
            return;
        }
        bool Wait(int timeMs)
        {
            if (m_threadHandle != INVALID_HANDLE_VALUE)
                return ::WaitForSingleObject(m_threadHandle, (DWORD)timeMs) == WAIT_OBJECT_0;
            return true; // ≈сли треда нет, считаем, что он успешно вышел
        }
        uint32_t SetAffinity(uint32_t mask)
        {
            if (m_threadHandle != INVALID_HANDLE_VALUE)
                return (uint32_t)::SetThreadAffinityMask(m_threadHandle, mask);
            return 0;
        }
        bool SetThreadPriority(ThreadPrior prior)
        {
            if (m_threadHandle == INVALID_HANDLE_VALUE)
                return false;

            int priorWin = THREAD_PRIORITY_NORMAL;
            switch (prior)
            {
            case vh::THREAD_PRIOR_LOW:
                priorWin = THREAD_PRIORITY_LOWEST;
                break;
            case vh::THREAD_PRIOR_MEDIUM:
                priorWin = THREAD_PRIORITY_BELOW_NORMAL;
                break;
            case vh::THREAD_PRIOR_NORMAL:
                priorWin = THREAD_PRIORITY_NORMAL;
                break;
            case vh::THREAD_PRIOR_HIGH:
                priorWin = THREAD_PRIORITY_ABOVE_NORMAL;
                break;
            case vh::THREAD_PRIOR_ULTRA:
                priorWin = THREAD_PRIORITY_HIGHEST;
                break;
            case vh::THREAD_PRIOR_REALTIME:
                priorWin = THREAD_PRIORITY_TIME_CRITICAL;
                break;
            }
            return ::SetThreadPriority(m_threadHandle, priorWin) ? true : false;
        }

    private:
        HANDLE m_threadHandle;
        unsigned int m_threadId;
        Thread::ThreadFunc m_cbFunc;

        static unsigned __stdcall ThreadProc(void* param)
        {
            Thread* pThis = (Thread*)param;
            int retCode = pThis->DoExecute();
            return (unsigned)retCode;
        }
    };

    static_assert(sizeof(ThreadImpl) == CTHREAD_IMPL_SIZE, "");

} // namespace vh
