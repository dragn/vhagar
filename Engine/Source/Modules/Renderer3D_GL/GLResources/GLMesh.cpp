#include "Modules/VhModules_PCH.hpp"
#include "GLMesh.hpp"

#include "Modules/Renderer/MRenderer3D_Old.hpp"
#include "Modules/ResourceSystem/ReadWrite.hpp"
#include "Modules/Renderer3D_GL/ImportUtils.hpp"

using namespace vh;

GLMesh::~GLMesh()
{
    if (IsLoaded())
    {
        LOG(ERROR) << "Not all rendered usage of mesh was released!";
    }

    if (mIndexSize > 0) delete[] mIndexData;
    if (mVertexCount > 0) delete[] mAttribData;

    if (mTexDta != nullptr)
    {
        SafeDeleteArray(mTexDta);
        mTexH = 0;
        mTexW = 0;
    }
}

void vh::GLMesh::SetTexture(GLuint* rgbData, GLsizei texW, GLsizei texH)
{
    mTexDta = rgbData;
    mTexW = texW;
    mTexH = texH;

    LOG(INFO) << "Added texture data " << texW << "x" << texH;
}

void vh::GLMesh::GetTexture(GLuint*& rgbaData, GLsizei& width, GLsizei& height) const
{
    rgbaData = mTexDta;
    width = mTexW;
    height = mTexH;
}

void vh::GLMesh::SetDim(GLuint dim)
{
    if (dim != 3 && dim != 4)
    {
        LOG(ERROR) << "Unsupported dimensions: " << dim;
        return;
    }

    if (dim != mDim)
    {
        mDim = dim;
    }
}

GLuint vh::GLMesh::GetDim() const
{
    return mDim;
}

GLuint vh::GLMesh::GetAttribDataSize() const
{
    return mVertexCount * (mDim + mAttribCount * 3);
}

const vh::GLBuffers vh::GLMesh::GetBufferInfo() const
{
    return mGLInfo;
}

bool vh::GLMesh::DoLoad()
{
    LOG(INFO) << "Mesh load " << this;

    CHECK(mGLInfo.attribBuffer == 0);
    CHECK(mGLInfo.indexBuffer == 0);
    CHECK(mGLInfo.texture == 0);

    if (mDim == 4)
    {
        mShaderId = Utils::GetShaderProgram("SimpleShader4");
    }
    else
    {
        mShaderId = Utils::GetShaderProgram("SimpleShader");
    }

    if (mShaderId < 0) return false;

    // specify sizes
    mGLInfo.attribCount = mAttribCount;
    mGLInfo.attribBufferSize = GetAttribDataSize();
    mGLInfo.indexBufferSize = mIndexSize;

    // buffer index data
    glGenBuffers(1, &mGLInfo.indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLInfo.indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mGLInfo.indexBufferSize * sizeof(GLuint), mIndexData, GL_STATIC_DRAW);

    // buffer attribute data
    glGenBuffers(1, &mGLInfo.attribBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mGLInfo.attribBuffer);
    glBufferData(GL_ARRAY_BUFFER, mGLInfo.attribBufferSize * sizeof(GLfloat), mAttribData, GL_STATIC_DRAW);

    // -- load texture
    if (mTexDta != nullptr)
    {
        glGenTextures(1, &mGLInfo.texture);
        glBindTexture(GL_TEXTURE_2D, mGLInfo.texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTexW, mTexH, 0, GL_RGBA, GL_UNSIGNED_BYTE, mTexDta);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        LOG(INFO) << "Texture loaded with ID: " << mGLInfo.texture;
    }

    return true;
}

bool vh::GLMesh::DoUnload()
{
    LOG(INFO) << "Mesh unload " << this;

    GLuint ids[] = { mGLInfo.indexBuffer, mGLInfo.attribBuffer };
    glDeleteBuffers(2, ids);
    glDeleteTextures(1, &mGLInfo.texture);

    mGLInfo.indexBuffer = 0;
    mGLInfo.indexBufferSize = 0;
    mGLInfo.attribBuffer = 0;
    mGLInfo.attribBufferSize = 0;
    mGLInfo.texture = 0;

    return true;
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
template<> bool vh::MResourceSystem::Load(const char* path, std::shared_ptr<vh::GLMesh> mesh)
{
    CHECK(mesh);

    std::regex objRegex(".*\\.obj");
    std::regex vhRegex(".*\\.vhmesh");
    std::cmatch match;

    if (std::regex_match(path, match, objRegex))
    {
        return vh::Utils::ImportWavefront(mesh.get(), path);
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
    Save mesh as binary data
    Data layout:
        - sizeof(GLuint) bytes index size
        - sizeof(GLuint) bytes attrib size
        - sizeof(GLuint) bytes attrib count
        - sizeof(GLint) * index size bytes index data
        - sizeof(GLfloat) * attrib size * attrib count bytes attrib data
*/
template<> bool vh::MResourceSystem::Save(const char* path, std::shared_ptr<const vh::GLMesh> mesh)
{
    std::ofstream stream;

    stream.open(GetFullPath(path), std::ios_base::out | std::ios_base::binary);
    if (!stream.is_open())
    {
        LOG(WARNING) << "Could not open file for writing: " << GetFullPath(path);
    }

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
