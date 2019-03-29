#include "Vhagar_PCH.hpp"
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

VH_COMPONENT_IMPL(vh::Physics);

vh::Physics::Physics()
    : Component(TickFrequency::NORMAL)
    , mFoundation(nullptr)
    , mPhysics(nullptr)
{

}

vh::Ret vh::Physics::TickInit(uint32_t delta)
{
    static ErrorCallback gErrorCallback;
    static PxDefaultAllocator gDefaultAllocatorCallback;

    mFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gErrorCallback);
    if (!mFoundation)
    {
        LOG(FATAL) << "PxCreateFoundation failed!";
        return Ret::FAILURE;
    }

    mPhysics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *mFoundation, mScale);
    if (!mPhysics)
    {
        LOG(FATAL) << "PxCreatePhysics failed!";
        return Ret::FAILURE;
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
        return Ret::FAILURE;
    }

    mControllerManager = PxCreateControllerManager(*mScene);

    return Ret::SUCCESS;
}

vh::Ret vh::Physics::TickClose(uint32_t delta)
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

    return Ret::SUCCESS;
}

void vh::Physics::StartFrame()
{
    if (IsRunning())
    {
        CHECK(mScene);
        mScene->simulate(GetTickStep() / 1000.0f);
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
