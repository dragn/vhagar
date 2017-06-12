#include "Common.hpp"
#include "Physics.hpp"
#include "Actor/Actor.hpp"

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
    mScene->setFlag(PxSceneFlag::eENABLE_ACTIVETRANSFORMS, true);
    if (!mScene)
    {
        LOG(FATAL) << "CreateScene failed!";
        Close();
        return;
    }

    mControllerManager = PxCreateControllerManager(*mScene);

    FinishInit();
}

void vh::Physics::TickClose(uint32_t delta)
{
    if (mControllerManager != nullptr)
    {
        mControllerManager->release();
        mControllerManager = nullptr;
    }
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
}

void vh::Physics::StartFrame()
{
    if (IsRunning())
    {
        CHECK(mScene);
        mScene->simulate(1.0f / 60.0f);
        mSimCalled = true;
    }
}

void vh::Physics::EndFrame()
{
    if (IsRunning() && mSimCalled)
    {
        CHECK(mScene);
        mScene->fetchResults(true);
        mSimCalled = false;

        // Update actors positions
        PxU32 nbActiveTransforms;
        const PxActiveTransform* activeTransforms = mScene->getActiveTransforms(nbActiveTransforms);
        for (PxU32 i = 0; i < nbActiveTransforms; ++i)
        {
            vh::Actor* actor = static_cast<vh::Actor*>(activeTransforms[i].userData);
            if (actor)
            {
                actor->SetPos(FromPhysX(activeTransforms[i].actor2World.p));
                actor->SetRot(FromPhysX(activeTransforms[i].actor2World.q));
            }
        }
    }
}
