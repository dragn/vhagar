#pragma once

#include "Geometry.hpp"
#include "Core/Actor/ActorBehavior.hpp"
#include "Physics.hpp"

namespace vh
{

class PhysicsBehavior : public ActorBehavior
{
public:
    PhysicsBehavior(bool isStatic = true);

    // -- ActorBehavior interface
    virtual void StartPlay() override;
    virtual void EndPlay() override;
    virtual void Tick(uint32_t delta) override;

    bool IsStatic() const;
    bool IsDynamic() const;

    void SetBoxGeometry(V3 extents);
    void SetCapsuleGeometry(float radius, float halfHeight);
    void SetSphereGeometry(float radius);

    physx::PxRigidActor* GetRigidActor()
    {
        return mActor;
    }

private:
    bool mIsStatic;
    std::unique_ptr<Geometry> mCollisionGeometry;

    Physics* mPhysics;
    physx::PxRigidActor* mActor;
    physx::PxMaterial* mMaterial;
};

} // namespace vh
