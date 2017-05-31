#pragma once

#include "App/App.hpp"
#include "Renderable/Mesh.hpp"
#include <fstream>

namespace vh
{

class ResourceSystem : public Component
{
    COMPONENT_NAME(ResourceSystem);

public:
    ResourceSystem() : Component() {}

    template<typename T>
    bool Load(const char* path, T* res);

    template<typename T>
    bool Save(const char* path, const T* res);

    std::shared_ptr<Mesh> GetMesh(const char* name);

    virtual void TickInit(uint32_t delta);
    virtual void TickClose(uint32_t delta);

private:
    std::unordered_map<std::string, std::shared_ptr<Mesh>> mMeshStorage;
};

template<> bool ResourceSystem::Load(const char* path, Mesh* mesh);
template<> bool ResourceSystem::Save(const char* path, const Mesh* mesh);

}
