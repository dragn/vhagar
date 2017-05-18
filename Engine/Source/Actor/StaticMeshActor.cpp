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
    mMesh = std::make_unique<Mesh>();
    std::regex objRegex(".*\\.obj");
    std::regex vhRegex(".*\\.vhmesh");
    std::cmatch match;
    if (std::regex_match(filename, match, objRegex))
    {
        Utils::ImportWavefront(mMesh.get(), filename);
    }
    else if (std::regex_match(filename, match, vhRegex))
    {
        ResourceSystem* resource = App::Get<ResourceSystem>();
        CHECK(resource);
        resource->Load(filename, mMesh.get());
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
    if (mMesh)
    {
        mMesh->SetModel(mTransform);
        App::Get<Renderer>()->AddObject(mMesh.get());
    }
}

void StaticMeshActor::OnDestroy()
{
    if (mMesh)
    {
        App::Get<Renderer>()->RemoveObject(mMesh.get());
        mMesh = nullptr;
    }
}


void StaticMeshActor::SetMesh(Mesh* mesh)
{
    if (mMesh)
    {
        App::Get<Renderer>()->RemoveObject(mMesh.get());
    }

    mMesh.reset(mesh);
    mMesh->SetModel(mTransform);
    App::Get<Renderer>()->AddObject(mMesh.get());
}

void StaticMeshActor::_UpdateTransform()
{
    Actor::_UpdateTransform();
    if (mMesh != nullptr) mMesh->SetModel(mTransform);
}

} // namespace vh
