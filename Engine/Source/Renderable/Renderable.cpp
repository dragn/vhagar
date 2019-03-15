#include "Vhagar_PCH.hpp"
#include "Renderable.hpp"

namespace vh
{

void Renderable::AddRef()
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

void Renderable::ReleaseRef()
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
