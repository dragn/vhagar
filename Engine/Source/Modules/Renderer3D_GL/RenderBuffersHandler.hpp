#pragma once

#include "Modules/Renderer3D_GL/RenderBuffer.hpp"
#include "Modules/standard.hpp"

namespace vh
{

class RenderBufferHandler
{
public:
    RenderBufferHandler();

    // Lock/Unlock buffers for reading
    // called from render thread
    void LockRead();
    void UnlockRead();

    // Get buffers for reading
    // Called from render thread
    const RenderBuffer& GetLastBuffer();
    const RenderBuffer& GetCurrentBuffer();

    // Get next buffer (writable)
    // Must only be called from main thread, access is not locked
    RenderBuffer& GetNextBuffer();

    // Proceed to next buffer
    // Blocks while trying to acquire a read lock
    // Called from main thread when buffers are filled for current frame
    void FlipBuffers();

private:
    size_t mLastIdx = 0;
    size_t mCurIdx = 1;
    size_t mNextIdx = 2;

    uint32_t mFrameIdx = 0;

    RenderBuffer mBuffers[3];

    std::mutex mReadMtx;
};

} // namespace vh
