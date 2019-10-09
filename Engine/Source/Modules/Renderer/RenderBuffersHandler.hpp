#pragma once

#include "Modules/Renderer/RenderBuffer.hpp"
#include "Modules/standard.hpp"

namespace vh
{

class RenderBufferHandler
{
public:
    void Create();
    void Destroy();

    // Lock/Unlock buffers for reading
    // called from render thread
    void LockRead();
    void UnlockRead();

    // Get buffers for reading
    // Called from render thread
    const RenderBuffer* GetLastBuffer();
    const RenderBuffer* GetCurrentBuffer();

    // Get next buffer (writable)
    // Must only be called from main thread, access is not locked
    RenderBuffer* GetNextBuffer();

    // Proceed to next buffer
    // Blocks while trying to acquire a read lock
    // Called from main thread when buffers are filled for current frame
    void FlipBuffers();

    // Allocate new block
    RenderBlock* AllocateNewBlock();

private:
    size_t mLastIdx = 0;
    size_t mCurIdx = 1;
    size_t mNextIdx = 2;

    RenderBuffer* mBuffers = nullptr;

    std::mutex mReadMtx;
};

} // namespace vh
