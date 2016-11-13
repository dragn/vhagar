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

StaticMeshActor::~StaticMeshActor()
{
    if (mMesh)
    {
        LOG(ERROR) << "StaticMeshActor OnDestroy was not called!";
    }
}

void StaticMeshActor::OnCreate()
{
    Renderer* render = App::Get<Renderer>();
    if (render)
    {
        mMesh->setModel(mTransform);
        render->AddObject(mMesh);
    }
}

void StaticMeshActor::OnDestroy()
{
    Renderer* render = App::Get<Renderer>();
    if (render)
    {
        render->RemoveObject(mMesh);
    }
    delete mMesh;
    mMesh = nullptr;
}

void StaticMeshActor::_UpdateTransform()
{
    Actor::_UpdateTransform();
    mMesh->setModel(mTransform);
}

} // namespace vh
