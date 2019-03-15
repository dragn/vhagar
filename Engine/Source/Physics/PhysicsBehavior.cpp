#include "Vhagar_PCH.hpp"
#include "PhysicsBehavior.hpp"
#include "App/App.hpp"
#include "Actor/Actor.hpp"

using namespace vh;
using namespace physx;

PhysicsBehavior::PhysicsBehavior(bool isStatic /*= true*/)
    : mIsStatic(isStatic)
{}

void PhysicsBehavior::StartPlay()
{
    mPhysics = App::Get<Physics>();
    CHECK(mPhysics);

    if (!mCollisionGeometry)
    {
        LOG(WARNING) << "No collision geometry specified, creating default";
        SetBoxGeometry(V3(1.0f));
    }

    PxTransform pose;
    pose.p = ToPhysX(mOwner->GetPos());
    pose.q = ToPhysX(mOwner->GetQuat());
    if (mIsStatic)
    {
        mActor = mPhysics->mPhysics->createRigidStatic(pose);
    }
    else
    {
        PxRigidDynamic* dyn = mPhysics->mPhysics->createRigidDynamic(pose);
        dyn->setMass(3.0f);
        mActor = dyn;
    }

    if (mActor == nullptr)
    {
        LOG(ERROR) << "Could not create physx actor!";
        return;
    }

    mActor->userData = (void*) mOwner;

    mMaterial = mPhysics->mPhysics->createMaterial(1.0, 1.0, 0.5);

    BoxGeometry* box = nullptr;
    SphereGeometry* sphere = nullptr;
    CapsuleGeometry* capsule = nullptr;
    PxShape* shape;
    switch (mCollisionGeometry->GetType())
    {
    case GeometryType::Box:
        box = static_cast<BoxGeometry*>(mCollisionGeometry.get());
        shape = mActor->createShape(PxBoxGeometry(box->GetExtents().x, box->GetExtents().y, box->GetExtents().z), *mMaterial);
        break;
    case GeometryType::Sphere:
        sphere = static_cast<SphereGeometry*>(mCollisionGeometry.get());
        shape = mActor->createShape(PxSphereGeometry(sphere->GetRadius()), *mMaterial);
        break;
    case GeometryType::Capsule:
        capsule = static_cast<CapsuleGeometry*>(mCollisionGeometry.get());
        shape = mActor->createShape(PxCapsuleGeometry(capsule->GetRadius(), capsule->GetHalfHeight()), *mMaterial);
        break;
    }

    mPhysics->mScene->addActor(*mActor);
}

void PhysicsBehavior::SetBoxGeometry(V3 extents)
{
    mCollisionGeometry = std::unique_ptr<Geometry>(new BoxGeometry(extents));
}

void PhysicsBehavior::SetCapsuleGeometry(float radius, float halfHeight)
{
    mCollisionGeometry = std::unique_ptr<Geometry>(new CapsuleGeometry(radius, halfHeight));
}

void PhysicsBehavior::SetSphereGeometry(float radius)
{
    mCollisionGeometry = std::unique_ptr<Geometry>(new SphereGeometry(radius));
}

void vh::PhysicsBehavior::EndPlay()
{
    // TODO react to physics closing properly
    if (!mPhysics->IsRunning())
    {
        return;
    }

    if (mActor)
    {
        mPhysics->mScene->removeActor(*mActor);

        mActor->release();
        mActor = nullptr;
    }

    if (mMaterial)
    {
        mMaterial->release();
        mMaterial = nullptr;
    }
}

void vh::PhysicsBehavior::Tick(uint32_t delta)
{
}
