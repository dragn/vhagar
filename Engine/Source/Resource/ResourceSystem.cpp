#include "Common.hpp"

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

static std::string currDir;

using namespace vh;

template<typename T>
bool OpenFile(T& stream, const std::string& filename, std::ios_base::openmode mode)
{
    stream.open(filename, mode);

    if (!stream.is_open())
    {
        LOG(WARNING) << "Could not open file: " << filename;
    }

    return stream.is_open();
}

std::string GetFullPath(const char* relPath)
{
    std::string fullPath(currDir);
    fullPath.append("/");
    fullPath.append(relPath);
    return fullPath;
}

template<typename T>
void Write(std::ostream& stream, const T* obj, size_t count = 1)
{
    stream.write(reinterpret_cast<const char*>(obj), sizeof(T) * count);
}

template<typename T>
void Read(std::istream& stream, T* obj, size_t count = 1)
{
    stream.read(reinterpret_cast<char*>(obj), sizeof(T) * count);
}

/*
    Load mesh from binary stream
    Data layout:
        - sizeof(GLuint) bytes index size
        - sizeof(GLuint) bytes attrib size
        - sizeof(GLuint) bytes attrib count
        - sizeof(GLint) * index size bytes index data
        - sizeof(GLfloat) * attrib size * attrib count bytes attrib data
*/
template<> bool vh::ResourceSystem::Load(const char* path, Mesh* mesh)
{
    CHECK(mesh);

    std::regex objRegex(".*\\.obj");
    std::regex vhRegex(".*\\.vhmesh");
    std::cmatch match;

    if (std::regex_match(path, match, objRegex))
    {
        return Utils::ImportWavefront(mesh, path);
    }
    else if (!std::regex_match(path, match, vhRegex))
    {
        LOG(ERROR) << "Unsupported mesh file: " << path;
        return false;
    }

    std::ifstream stream;

    std::string fullPath = GetFullPath(path);

    if (!OpenFile(stream, fullPath, std::ios_base::in | std::ios_base::binary)) return false;

    GLuint vertexCount, attrCount, indexSize, dim;
    GLfloat* attrData = nullptr;
    GLuint* indexData = nullptr;

    Read(stream, &dim);
    CHECK(dim == 3 || dim == 4) << "invalid dimensions";

    Read(stream, &indexSize);
    Read(stream, &vertexCount);
    Read(stream, &attrCount);

    indexData = new GLuint[indexSize];
    Read(stream, indexData, indexSize);
    if (stream.fail() || stream.eof())
    {
        LOG(FATAL) << "Resource load error";
        return false;
    }

    size_t dataSize = vertexCount * (dim + 3 * attrCount);
    attrData = new GLfloat[dataSize];
    Read(stream, attrData, dataSize);
    if (stream.fail())
    {
        LOG(FATAL) << "Resource load error";
        delete[] attrData;
        return false;
    }

    mesh->SetDim(dim);
    mesh->SetAttribData(vertexCount, attrCount, attrData);
    mesh->SetIndexData(indexSize, indexData);

    // -- load texture
    if (!stream.eof())
    {
        GLsizei texW, texH;
        Read(stream, &texW);
        Read(stream, &texH);
        if (texW > 0 && texH > 0)
        {
            GLuint* data = new GLuint[texW * texH];
            Read(stream, data, texW * texH);
            mesh->SetTexture(data, texW, texH);
        }
    }

    stream.close();

    return true;
}

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
bool vh::ResourceSystem::Save(const char* path, const Actor* actor)
{
    return false;
}

template<>
bool vh::ResourceSystem::Load(const char* path, Actor* actor)
{
    return false;
}

/*
    Save mesh as binary data
    Data layout:
        - sizeof(GLuint) bytes index size
        - sizeof(GLuint) bytes attrib size
        - sizeof(GLuint) bytes attrib count
        - sizeof(GLint) * index size bytes index data
        - sizeof(GLfloat) * attrib size * attrib count bytes attrib data
*/
template<> bool vh::ResourceSystem::Save(const char* path, const Mesh* mesh)
{
    std::ofstream stream;

    if (!OpenFile(stream, GetFullPath(path), std::ios_base::out | std::ios_base::binary)) return false;

    CHECK(mesh);

    GLuint vertexCount, attrCount, indexSize;
    GLuint dim = mesh->GetDim();
    GLfloat* attrData = nullptr;
    GLuint* indexData = nullptr;

    mesh->GetAttribData(vertexCount, attrCount, attrData);
    mesh->GetIndexData(indexSize, indexData);

    Write(stream, &dim);
    Write(stream, &indexSize);
    Write(stream, &vertexCount);
    Write(stream, &attrCount);

    Write(stream, indexData, indexSize);
    Write(stream, attrData, mesh->GetAttribDataSize());

    // -- serializing textures
    GLuint* texDta;
    GLsizei texW, texH;
    mesh->GetTexture(texDta, texW, texH);

    Write(stream, &texW);
    Write(stream, &texH);
    if (texDta != nullptr) Write(stream, texDta, texW * texH);

    stream.close();

    return true;
}

std::shared_ptr<vh::Mesh> vh::ResourceSystem::GetMesh(const char* name)
{
    auto iter = mMeshStorage.find(name);
    if (iter == mMeshStorage.end())
    {
        std::shared_ptr<vh::Mesh> ptr = std::make_shared<vh::Mesh>();
        if (!ptr)
        {
            LOG(FATAL) << "Unable to allocate mesh";
            return nullptr;
        }
        mMeshStorage[std::string(name)] = ptr;
        if (Load<vh::Mesh>(name, ptr.get()))
        {
            LOG(INFO) << "Loaded mesh " << name;
            return ptr;
        }
        else
        {
            LOG(FATAL) << "Unable to load mesh " << name;
            return nullptr;
        }
    }
    else
    {
        return iter->second;
    }
}

void vh::ResourceSystem::TickInit(uint32_t delta)
{
    char cd[FILENAME_MAX];
    GetCurrentDir(cd, FILENAME_MAX);

    currDir = cd;
    std::replace(currDir.begin(), currDir.end(), '\\', '/');

    // preload meshes from Assets/Meshes
    std::vector<std::string> names;
    vh::Utils::ListFiles("Assets/Meshes", names);
    for (const std::string& name : names)
    {
        std::shared_ptr<Mesh> ptr = std::make_shared<Mesh>();
        if (Load<Mesh>(name.c_str(), ptr.get()))
        {
            LOG(INFO) << "Preloaded mesh " << name;
            mMeshStorage[name] = ptr;
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
    mMeshStorage.clear();

    FinishClose();
}
