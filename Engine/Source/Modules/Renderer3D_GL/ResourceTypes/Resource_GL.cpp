#include "Modules/VhModules_PCH.hpp"
#include "Resource_GL.hpp"

namespace vh
{

void Resource_GL::AddRef()
{
    if (mRefCount == 0)
    {
        bool result = DoLoad();
        if (result)
        {
            mLoaded = true;
        }
        else
        {
            LOG(ERROR) << "Could not load renderable";
        }
    }
    mRefCount++;
}

void Resource_GL::ReleaseRef()
{
    if (mRefCount != 0)
    {
        mRefCount--;
        if (mRefCount == 0)
        {
            DoUnload();
            mLoaded = false;
        }
    }
}

} // namespace vh
