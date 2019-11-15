#pragma once

#include "Modules/ResourceSystem/Resource.hpp"
#include "Modules/standard.hpp"

namespace vh
{
    class GLResource
    {
        friend class MRenderer3D_GL_Thread;

    public:
        bool IsLoaded() const { return mLoaded.load(); }

    protected:
        virtual bool DoLoad() = 0;
        virtual bool DoUnload() = 0;

    private:
        // -- Update ref count is only accessible through the render thread
        void AddRef();
        void ReleaseRef();
        // --

        int32_t id = -1;
        std::atomic<bool> mLoaded = false;

        // Counts the rendered references.
        // Renderable is automatically buffered when the value changed from zero
        // and removed from render context, when value reaches zero again.
        uint32_t mRefCount = 0;
    };

}
