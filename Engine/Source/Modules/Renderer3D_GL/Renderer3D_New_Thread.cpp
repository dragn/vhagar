#include "Modules/VhModules_PCH.hpp"
#include "Renderer3D_New_Thread.hpp"

namespace vh
{
    void MRenderer3D_New_Thread::Start()
    {
        CHECK(!IsStarted());
        CHECK(!IsReady());

        mThread.Start([](void* _this)->int { reinterpret_cast<MRenderer3D_New_Thread*>(_this)->Run(); return 0; }, this);

        mStarted = true;
    }

    void MRenderer3D_New_Thread::Stop(bool wait /*= true*/)
    {
        if (IsStarted())
        {
            mRequestStop.store(true);
            if (wait)
            {
                mThread.Wait(-1);
            }
        }
    }

    vh::RenderBuffer& MRenderer3D_New_Thread::GetWriteBuffer()
    {
        return mRenderBuffers.GetNextBuffer();
    }

    void MRenderer3D_New_Thread::FlipBuffers()
    {
        mRenderBuffers.FlipBuffers();
    }

    void MRenderer3D_New_Thread::Run()
    {

    }

} // namespace vh