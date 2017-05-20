#include "Common.hpp"
#include "SkyBoxBehavior.hpp"
#include "App/App.hpp"

using namespace vh;

namespace
{
const GLfloat vertexData[] = { -1, -1, -1, -1, -1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1, 1, 1, 1, -1, -1, 1, -1, 1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1, -1, -1, -1, 1, -1, -1, 1, 1, -1, 1, -1, -1, -1, -1, 1, -1, -1, 1, -1, -1, -1, 1, -1, 1, 1, -1, 1, 1, 1, -1, -1, 1, 1, 1, -1 };
const GLuint vertexDataSize = 72;
const GLuint indexData[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 0, 2, 19, 3, 5, 20, 6, 8, 21, 9, 11, 22, 12, 14, 23, 15, 17 };
const GLuint indexDataSize = 36;
}

SkyBoxBehavior::SkyBoxBehavior(Actor* owner, const Utils::CubeMap& cubeMap)
    : ActorBehavior(owner)
    , mCubeMap(cubeMap)
{}

void SkyBoxBehavior::StartPlay()
{
    mTexture = Utils::LoadCubeMapTexture(mCubeMap);
    mProgramID = Utils::GetShaderProgram("SkyDome");

    mGLInfo.attribCount = 1;
    mGLInfo.attribBufferSize = vertexDataSize;
    mGLInfo.attribBuffer = Utils::BufferData(vertexDataSize, vertexData);

    mGLInfo.indexBufferSize = indexDataSize;
    mGLInfo.indexBuffer = Utils::BufferElementArray(indexDataSize, indexData);

    mRenderer = App::Get<Renderer>();
    CHECK(mRenderer);
}

void SkyBoxBehavior::EndPlay()
{
    GLuint ids[] = { mGLInfo.indexBuffer, mGLInfo.attribBuffer };
    glDeleteBuffers(2, ids);
    glDeleteTextures(1, &mTexture);
}

void SkyBoxBehavior::Tick(uint32_t delta)
{
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
    glUseProgram(mProgramID);

    M4 view = mRenderer->GetView();
    view[3] = { 0, 0, 0, 1 }; // remove translation from view

    M4 MVP = mRenderer->GetProjection() * view * glm::scale(M4(1.0f), V3(50.f, 50.f, 50.f));
    Utils::PutUniformMat4(mProgramID, "uMVP", MVP);

    glBindBuffer(GL_ARRAY_BUFFER, mGLInfo.attribBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLInfo.indexBuffer);
    glDrawElements(GL_TRIANGLES, mGLInfo.indexBufferSize, GL_UNSIGNED_INT, (void*) 0);

    glDisableVertexAttribArray(0);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
}

