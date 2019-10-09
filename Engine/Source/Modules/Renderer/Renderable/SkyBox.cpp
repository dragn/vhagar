#include "Modules/VhModules_PCH.hpp"
#include "SkyBox.hpp"

namespace
{
const GLfloat vertexData[] = { -1, -1, -1, -1, -1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1, -1, 1, -1, 1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, -1, -1, 1, -1, -1, 1, 1, -1, 1, -1, -1, -1, -1, 1, -1, -1, 1, -1, -1, -1, 1, -1, 1, 1, -1, 1, 1, 1, -1, -1, 1, 1, 1, -1 };
const GLuint vertexDataSize = 72;
const GLuint indexData[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 0, 2, 19, 3, 5, 20, 6, 8, 21, 9, 11, 22, 12, 14, 23, 15, 17 };
const GLuint indexDataSize = 36;
}

bool vh::SkyBox::DoLoad()
{
    mGLInfo.texture = Utils::LoadCubeMapTexture(mCubeMap);
    mShaderId = Utils::GetShaderProgram("SkyDome");

    mGLInfo.attribCount = 1;
    mGLInfo.attribBufferSize = vertexDataSize;
    mGLInfo.attribBuffer = Utils::BufferData(vertexDataSize, vertexData);

    mGLInfo.indexBufferSize = indexDataSize;
    mGLInfo.indexBuffer = Utils::BufferElementArray(indexDataSize, indexData);

    return true;
}

bool vh::SkyBox::DoUnload()
{
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
