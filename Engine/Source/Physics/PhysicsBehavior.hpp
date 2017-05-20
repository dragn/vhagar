#pragma once

#include "Geometry.hpp"

namespace vh
{

class PhysicsBehavior
{
public:
    PhysicsBehavior();
    ~PhysicsBehavior();

    bool IsStatic() const;
    bool IsDynamic() const;

    void SetCollisionGeometry(Geometry*);

private:
    bool mIsStatic = true;
    Geometry* mCollisionGeometry = nullptr;
};

} // namespace vh
