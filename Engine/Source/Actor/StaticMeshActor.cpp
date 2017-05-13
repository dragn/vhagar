#include "Common.hpp"

#include "StaticMeshActor.hpp"
#include "App/App.hpp"
#include "Utils/ImportUtils.hpp"
#include "Renderer/Renderer.hpp"
#include "Resource/ResourceSystem.hpp"
#include <regex>

namespace vh
{

StaticMeshActor::StaticMeshActor(const char* filename)
{
    mMesh = new Mesh;
    std::regex objRegex(".*\\.obj");
    std::regex vhRegex(".*\\.vhmesh");
    std::cmatch match;
    if (std::regex_match(filename, match, objRegex))
    {
        Utils::ImportWavefront(mMesh, filename);
    }
    else if (std::regex_match(filename, match, vhRegex))
    {
        ResourceSystem* resource = App::Get<ResourceSystem>();
        CHECK(resource);
        resource->Load(filename, mMesh);
    }
    else
    {
        LOG(ERROR) << "Unsupported mesh file: " << filename;
    }
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
        SafeDelete(mMesh);
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
