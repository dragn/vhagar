#include "Common.hpp"
#include "Renderable.hpp"

namespace vh
{

void Renderable::AddUsage()
{
    if (mRenderUsage == 0)
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
    mRenderUsage++;
}

void Renderable::ReleaseUsage()
{
    if (mRenderUsage != 0)
    {
        mRenderUsage--;
        if (mRenderUsage == 0)
        {
            DoUnload();
            mLoaded = false;
        }
    }
}

} // namespace vh
