#include "Vhagar_PCH.hpp"

#include <algorithm>
#include "ResourceSystem.hpp"
#include "Utils/ImportUtils.hpp"
#include "Utils/FileSystem.hpp"

#include <cstdio>
#ifdef CMAKE_PLATFORM_WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace vh;

VH_COMPONENT_IMPL(ResourceSystem);

/*
    Save actor as ASCII human-readable file *.vhactor

    First line must be
        ACTOR <name>

    Then follows any number of behaviors, one for each line
    (Note that not all behaviors are compatible with each other)

    Supported behaviors:

        Mesh:
            MESH <mesh name>

        Physics descriptor (only one allowed):
            PHYSICS ( STATIC | DYNAMIC ) <collision shape>

            Collision shape has one of three forms:
                BOX <x> <y> <z>
                SPHERE <radius>
                CAPSULE <radius> <half height>

        Capsule-based character controller (does not compatible with PHYSICS)
            CAPSULE_CHARACTER_CONTROLLER <radius> <half height>

        First-person camera:
            FIRST_PERSON_CAMERA <x> <y> <z>

        Point light:
            POINT_LIGHT <intensity>

    Example 1 (character):
        ACTOR Character
        MESH Assets/Meshes/char.vhmesh
        CAPSULE_CHARACTER_CONTROLLER 0.4 0.5
        FIRST_PERSON_CAMERA 0 0.25 0

    Example 2 (dynamic box):
        ACTOR DynamicBox
        MESH Assets/Meshes/box.vhmesh
        PHYSICS DYNAMIC BOX 1.0 1.0 1.0
*/
template<>
bool vh::ResourceSystem::Save(const char* path, std::shared_ptr<const Actor> actor)
{
    return false;
}

template<>
bool vh::ResourceSystem::Load(const char* path, std::shared_ptr<Actor> actor)
{
    return false;
}

void vh::ResourceSystem::TickInit(uint32_t delta)
{
    char cd[FILENAME_MAX];
    GetCurrentDir(cd, FILENAME_MAX);

    mCurrDir = cd;
    std::replace(mCurrDir.begin(), mCurrDir.end(), '\\', '/');

    // preload meshes from Assets/Meshes
    std::vector<std::string> names;
    vh::Utils::ListFiles("Assets/Meshes", names);
    for (const std::string& name : names)
    {
        std::shared_ptr<Mesh> ptr = std::make_shared<Mesh>();
        if (Load<Mesh>(name.c_str(), ptr))
        {
            LOG(INFO) << "Preloaded mesh " << name;
            mStorage[name] = ptr;
        }
        else
        {
            LOG(ERROR) << "Mesh preload failed " << name;
        }
    }

    FinishInit();
}

void vh::ResourceSystem::TickClose(uint32_t delta)
{
    mStorage.clear();

    FinishClose();
}

std::string ResourceSystem::GetFullPath(const char* relPath)
{
    std::string fullPath(mCurrDir);
    fullPath.append("/");
    fullPath.append(relPath);
    return fullPath;
}
