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
    std::ifstream stream;

    std::string fullPath = GetFullPath(path);

    CHECK(mesh);

    if (fullPath.find(".obj") == fullPath.size() - 4)
    {
        LOG(INFO) << "Importing wavefront object";
        return Utils::ImportWavefront(mesh, fullPath.c_str());
    }

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
        size_t texW, texH;
        Read(stream, &texW);
        Read(stream, &texH);
        if (texW > 0 && texH > 0)
        {
            uint32_t* data = new uint32_t[texW * texH];
            Read(stream, data, texW * texH);
            mesh->SetTexture(data, texW, texH);
        }
    }

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
    uint32_t* texDta;
    size_t texW, texH;
    mesh->GetTexture(texDta, texW, texH);
    Write(stream, &texW);
    Write(stream, &texH);
    if (texDta != nullptr) Write(stream, texDta, texW * texH);

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
