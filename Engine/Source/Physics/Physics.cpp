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

    mFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gErrorCallback);
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

    // -- Create scene
    PxSceneDesc sceneDesc(mScale);
    sceneDesc.gravity = PxVec3(0, -20.0f, 0);
    sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    mScene = mPhysics->createScene(sceneDesc);
    if (!mScene)
    {
        LOG(FATAL) << "CreateScene failed!";
        Close();
        return;
    }

    FinishInit();
}

void vh::Physics::TickClose(uint32_t delta)
{
    if (mScene != nullptr)
    {
        mScene->release();
        mScene = nullptr;
    }
    if (mPhysics != nullptr)
    {
        mPhysics->release();
        mPhysics = nullptr;
    }
    if (mFoundation != nullptr)
    {
        mFoundation->release();
        mFoundation = nullptr;
    }

    FinishClose();
}

void vh::Physics::TickRun(uint32_t delta)
{
    // the most simple simulation step
}

void vh::Physics::StartFrame()
{
    if (mScene) mScene->simulate(1.0f / 60.0f);
}

void vh::Physics::EndFrame()
{
    if (mScene) mScene->fetchResults(true);
}
