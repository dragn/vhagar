#include "Common.hpp"

#include "Utils/GLUtils.hpp"
#include "Mesh.hpp"

using namespace vh;

Mesh::~Mesh() {
    LOG(INFO) << "Mesh destructor";
    if (mIndexSize > 0) delete[] mIndexData;
    if (mAttribSize > 0) delete[] mAttribData;
    if (mTexture != NULL) SDL_FreeSurface(mTexture);
}

void
Mesh::BeforeRender() {
    if (isReadyToRender) {
        LOG(WARNING) << "Attempt to prepare already prepared object!";
        return;
    }

    if (mIndexSize == 0) {
        LOG(WARNING) << "Object has no data!";
        return;
    }

    mProgramID = Utils::GetShaderProgram("SimpleShader");

    if (mProgramID == 0) {
        LOG(ERROR) << "Object won't be rendered, because shader program invalid";
        // there was an error linking the program, do not proceed!
        return;
    }

    // specify sizes
    mGLInfo.attribBufferSize = mAttribSize;
    mGLInfo.indexBufferSize = mIndexSize;

    // buffer index data
    mGLInfo.indexBuffer = Utils::BufferElementArray(mGLInfo.indexBufferSize, mIndexData);

    // buffer attribute data
    glGenBuffers(1, &mGLInfo.attribBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mGLInfo.attribBuffer);

    GLuint bytes = mGLInfo.attribBufferSize * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, mAttribCount * bytes, NULL, GL_STATIC_DRAW);

    for (size_t i = 0; i < mAttribCount; i++) {
        glBufferSubData(GL_ARRAY_BUFFER, i * bytes, bytes, mAttribData + i * mAttribSize);
    }

    // load texture
    if (mTexture != NULL) {
        mGLInfo.texture = Utils::BufferTexture2D(mTexture);
        LOG(INFO) << "Texture loaded with ID: " << mGLInfo.texture;
    }

    isReadyToRender = true;
}

void
Mesh::AfterRender() {
    if (isReadyToRender) {
        GLuint ids[] = { mGLInfo.indexBuffer, mGLInfo.attribBuffer };
        glDeleteBuffers(2, ids);
        isReadyToRender = false;
    }
    if (mGLInfo.texture > 0) {
        LOG(INFO) << "Deleting previous texture: " << mGLInfo.texture;
        glDeleteTextures(1, &mGLInfo.texture);
    }
}

void
Mesh::Render(glm::mat4 projection, glm::mat4 view, const Light* lightSource) {

    if (!isReadyToRender) return;

    MVP = projection * view * mModel;

    glUseProgram(mProgramID);

    Utils::PutUniformVec3(mProgramID, "uLightPosition", lightSource->GetPos());
    Utils::PutUniformFloat(mProgramID, "uLightIntensity", lightSource->GetIntensity());

    Utils::PutUniformMat4(mProgramID, "uMVP", MVP);
    Utils::PutUniformMat4(mProgramID, "uM", mModel);
    Utils::PutUniformMat4(mProgramID, "uV", view);

    if (mGLInfo.texture) {
        glBindTexture(GL_TEXTURE_2D, mGLInfo.texture);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    for (GLuint i = 0; i < mAttribCount; i++) {
        glEnableVertexAttribArray(i);
    }

    glBindBuffer(GL_ARRAY_BUFFER, mGLInfo.attribBuffer);

    GLfloat *offset = 0;
    for (GLuint i = 0; i < mAttribCount; i++) {
        glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, offset + i * mGLInfo.attribBufferSize);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mGLInfo.indexBuffer);
    glDrawElements(GL_TRIANGLES, mGLInfo.indexBufferSize, GL_UNSIGNED_INT, (void*)0);

    for (GLuint i = 0; i < mAttribCount; i++) {
        glDisableVertexAttribArray(i);
    }
}

void Mesh::SetTexture(const std::string &filename) {
    if (mGLInfo.texture > 0) {
        LOG(INFO) << "Deleting previous texture: " << mGLInfo.texture;
        glDeleteTextures(1, &mGLInfo.texture);
        SDL_FreeSurface(mTexture);
    }

    LOG(INFO) << "Setting up texture " << filename;
    mTexture = Utils::LoadImage(filename);
}

void vh::Mesh::SetTexture(const SDL_Surface* texture)
{
    if (mGLInfo.texture > 0) {
        LOG(INFO) << "Deleting previous texture: " << mGLInfo.texture;
        glDeleteTextures(1, &mGLInfo.texture);
    }

    if (mTexture != nullptr)
    {
        SDL_FreeSurface(mTexture);
        mTexture = nullptr;
    }

    if (texture != nullptr) mTexture = new SDL_Surface(*texture);
}
