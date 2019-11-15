#include "Modules/VhModules_PCH.hpp"
#include "GLResource.hpp"

namespace vh
{

void GLResource::AddRef()
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

void GLResource::ReleaseRef()
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
