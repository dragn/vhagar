#pragma once

#include <string>
#include "Actor.hpp"
#include "Renderable/Mesh.hpp"

namespace vh
{

class StaticMeshActor : public Actor
{
public:
    StaticMeshActor();
    StaticMeshActor(const char* filename);

    virtual ~StaticMeshActor();

    // Called when actor is created
    virtual void OnCreate();

    // Called when actor is destroyed
    virtual void OnDestroy();

    void SetMesh(Mesh* mesh);
    const Mesh* GetMesh() const { return mMesh; }

    void LoadMesh(const char* path);

protected:
    virtual void _UpdateTransform();

private:
    Mesh* mMesh;
};

} // namespace vh
