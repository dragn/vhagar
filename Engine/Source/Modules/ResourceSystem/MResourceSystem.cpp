#include "Modules/VhModules_PCH.hpp"
#include "MResourceSystem.hpp"

#include "Modules/Renderer/ImportUtils.hpp"

using namespace vh;

VH_MODULE_IMPL(MResourceSystem);

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
bool vh::MResourceSystem::Save(const char* path, std::shared_ptr<const Actor> actor)
{
    return false;
}

template<>
bool vh::MResourceSystem::Load(const char* path, std::shared_ptr<Actor> actor)
{
    return false;
}

vh::Ret vh::MResourceSystem::TickInit(uint32_t delta)
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

    return Ret::SUCCESS;
}

vh::Ret vh::MResourceSystem::TickClose(uint32_t delta)
{
    mStorage.clear();

    return Ret::SUCCESS;
}

std::string MResourceSystem::GetFullPath(const char* relPath)
{
    std::string fullPath(mCurrDir);
    fullPath.append("/");
    fullPath.append(relPath);
    return fullPath;
}
