#include "Common.hpp"

#include "StaticMeshActor.hpp"
#include "App/App.hpp"
#include "Utils/ImportUtils.hpp"
#include "Renderer/Renderer.hpp"

namespace vh
{

StaticMeshActor::StaticMeshActor(const char* filename)
{
    mMesh = new Mesh;
    Utils::ImportWavefront(mMesh, filename);
}


StaticMeshActor::StaticMeshActor()
    : mMesh(nullptr)
{
}

StaticMeshActor::~StaticMeshActor()
{
    if (mMesh)
    {
        LOG(ERROR) << "StaticMeshActor OnDestroy was not called!";
    }
}

void StaticMeshActor::OnCreate()
{
    if (mMesh != nullptr)
    {
        mMesh->SetModel(mTransform);
        App::Get<Renderer>()->AddObject(mMesh);
    }
}

void StaticMeshActor::OnDestroy()
{
    if (mMesh != nullptr)
    {
        App::Get<Renderer>()->RemoveObject(mMesh);
        delete mMesh;
        mMesh = nullptr;
    }
}


void StaticMeshActor::SetMesh(Mesh* mesh)
{
    if (mMesh != nullptr)
    {
        App::Get<Renderer>()->RemoveObject(mMesh);
        delete mMesh;
        mMesh = nullptr;
    }

    mMesh = mesh;
    mMesh->SetModel(mTransform);
    App::Get<Renderer>()->AddObject(mMesh);
}

void StaticMeshActor::_UpdateTransform()
{
    Actor::_UpdateTransform();
    if (mMesh != nullptr) mMesh->SetModel(mTransform);
}

} // namespace vh
