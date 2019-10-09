#include "Modules/VhModules_PCH.hpp"
#include "RenderBuffersHandler.hpp"

namespace vh
{

void RenderBufferHandler::Create()
{
    mBuffers = new RenderBuffer[3];

    // set initial sizes
    mBuffers[0].header.size = 0;
    mBuffers[0].header.time = 0;
    mBuffers[1].header.size = 0;
    mBuffers[1].header.time = 0;
    mBuffers[2].header.time = 0;
    mBuffers[2].header.size = 0;
}

void RenderBufferHandler::Destroy()
{
    SafeDeleteArray(mBuffers);
}

void RenderBufferHandler::LockRead()
{
    mReadMtx.lock();
}

void RenderBufferHandler::UnlockRead()
{
    mReadMtx.unlock();
}

const vh::RenderBuffer* RenderBufferHandler::GetLastBuffer()
{
    CHECK(mBuffers != nullptr) << "Buffers are not initialized";

    return &mBuffers[mLastIdx];
}

const vh::RenderBuffer* RenderBufferHandler::GetCurrentBuffer()
{
    CHECK(mBuffers != nullptr) << "Buffers are not initialized";

    return &mBuffers[mCurIdx];
}

vh::RenderBuffer* RenderBufferHandler::GetNextBuffer()
{
    CHECK(mBuffers != nullptr) << "Buffers are not initialized";

    return &mBuffers[mNextIdx];
}

void RenderBufferHandler::FlipBuffers()
{
    mReadMtx.lock(); // blocks until render thread releases buffers

    // rotate buffer indexes
    mLastIdx = (mLastIdx + 1) % 3;
    mCurIdx = (mCurIdx + 1) % 3;
    mNextIdx = (mNextIdx + 1) % 3;

    mReadMtx.unlock();
}

vh::RenderBlock* RenderBufferHandler::AllocateNewBlock()
{
    if (mBuffers == nullptr) return nullptr;

    // must only be called from main thread, so no lock is necessary
    RenderBuffer& buffer = mBuffers[mNextIdx];

    if (buffer.header.size >= RenderBufferConstants::MAX_RENDER_BLOCKS)
    {
        LOG(WARNING) << "RenderBuffer overflow!";
        return nullptr;
    }
    buffer.header.size++;
    return &buffer.blocks[buffer.header.size - 1];
}

} // namespace vh

