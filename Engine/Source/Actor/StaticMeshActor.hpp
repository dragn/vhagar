#pragma once

#include <string>
#include "Actor.hpp"

namespace vh
{

class StaticMeshActor : public Actor
{
public:
    StaticMeshActor(const char* filename);

    virtual ~StaticMeshActor();

    // Called when actor is created
    virtual void OnCreate();

    // Called when actor is destroyed
    virtual void OnDestroy();

protected:
    virtual void _UpdateTransform();

private:
    Mesh* mMesh;
};

} // namespace vh
