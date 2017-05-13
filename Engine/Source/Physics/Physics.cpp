#include "Common.hpp"
#include "Physics.hpp"

using namespace physx;

namespace
{
class ErrorCallback : public PxErrorCallback
{
    virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) override
    {
        LOG(ERROR) << "PhysX Error at " << file << ":" << line << " " << message;
    }
};
}

vh::Physics::Physics()
    : Component(eTickFrequency::NORMAL)
    , mFoundation(nullptr)
    , mPhysics(nullptr)
{

}

void vh::Physics::TickInit(uint32_t delta)
{
    static ErrorCallback gErrorCallback;
    static PxDefaultAllocator gDefaultAllocatorCallback;

    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gErrorCallback);
    if (!mFoundation)
    {
        LOG(FATAL) << "PxCreateFoundation failed!";
        Close();
        return;
    }

    mPhysics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *mFoundation, mScale);
    if (!mPhysics)
    {
        LOG(FATAL) << "PxCreatePhysics failed!";
        Close();
        return;
    }

    // -- Register any additional features here

    FinishInit();
}

void vh::Physics::TickClose(uint32_t delta)
{
    if (mPhysics != nullptr) mPhysics->release();
    if (mFoundation != nullptr) mFoundation->release();

    FinishClose();
}

void vh::Physics::TickRun(uint32_t delta)
{
    // TODO run simulation
}
