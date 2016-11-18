#include "Common.hpp"

#include <algorithm>
#include "ResourceSystem.hpp"
#include "Utils/ImportUtils.hpp"

#include <cstdio>
#ifdef CMAKE_PLATFORM_WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

static std::string currDir;

const char* vh::ResourceSystem::COMPONENT_NAME = "ResourceSystem";

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
    std::ifstream stream;

    std::string fullPath = GetFullPath(path);

    CHECK(mesh);

    if (fullPath.find(".obj") == fullPath.size() - 4)
    {
        LOG(INFO) << "Importing wavefront object";
        return Utils::ImportWavefront(mesh, fullPath.c_str());
    }

    if (!OpenFile(stream, fullPath, std::ios_base::in | std::ios_base::binary)) return false;

    GLuint attrSize, attrCount, indexSize;
    GLfloat* attrData = nullptr;
    GLuint* indexData = nullptr;

    stream.read(reinterpret_cast<char*>(&indexSize), sizeof(GLuint));
    stream.read(reinterpret_cast<char*>(&attrSize), sizeof(GLuint));
    stream.read(reinterpret_cast<char*>(&attrCount), sizeof(GLuint));

    indexData = new GLuint[indexSize];
    stream.read(reinterpret_cast<char*>(indexData), sizeof(GLuint) * indexSize);
    if (stream.fail() || stream.eof())
    {
        LOG(FATAL) << "Resource load error";
        return false;
    }

    attrData = new GLfloat[attrCount * attrSize];
    stream.read(reinterpret_cast<char*>(attrData), sizeof(GLfloat) * attrCount * attrSize);
    if (stream.fail())
    {
        LOG(FATAL) << "Resource load error";
        return false;
    }

    mesh->SetAttribData(attrSize, attrCount, attrData);
    mesh->SetIndexData(indexSize, indexData);

    // TODO: load texture
    mesh->SetTexture(nullptr);

    stream.close();

    return true;
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

    GLuint attrSize, attrCount, indexSize;
    GLfloat* attrData = nullptr;
    GLuint* indexData = nullptr;

    mesh->GetAttribData(attrSize, attrCount, attrData);
    mesh->GetIndexData(indexSize, indexData);

    stream.write(reinterpret_cast<char*>(&indexSize), sizeof(GLuint));
    stream.write(reinterpret_cast<char*>(&attrSize), sizeof(GLuint));
    stream.write(reinterpret_cast<char*>(&attrCount), sizeof(GLuint));
    stream.write(reinterpret_cast<char*>(indexData), sizeof(GLuint) * indexSize);
    stream.write(reinterpret_cast<char*>(attrData), sizeof(GLfloat) * attrCount * attrSize);

    // TODO: save texture

    stream.close();

    return true;
}

void vh::ResourceSystem::TickInit(uint32_t delta)
{
    char cd[FILENAME_MAX];
    GetCurrentDir(cd, FILENAME_MAX);

    currDir = cd;
    std::replace(currDir.begin(), currDir.end(), '\\', '/');

    FinishInit();
}

void vh::ResourceSystem::TickClose(uint32_t delta)
{
    FinishClose();
}
