#pragma once

#include "Renderer/Lights.hpp"
#include "Renderable.hpp"

namespace vh {

class Mesh : public Renderable {

    friend class SkyBox;

public:
    Mesh() : mTexture(nullptr) {};

    virtual ~Mesh();

    /**
     * Sets Model matrix for this object
     */
    void SetModel(glm::mat4 _model) { mModel = _model; }

    void SetAttribData(GLuint size, GLuint count, GLfloat* data) {
        mAttribSize = size;
        mAttribCount = count;
        mAttribData = data;
    }

    void SetIndexData(GLuint size, GLuint* data) {
        mIndexSize = size;
        mIndexData = data;
    }

    void GetAttribData(GLuint& size, GLuint& count, GLfloat*& data) const
    {
        size = mAttribSize;
        count = mAttribCount;
        data = mAttribData;
    }

    void GetIndexData(GLuint& size, GLuint*& data) const
    {
        size = mIndexSize;
        data = mIndexData;
    }

    void SetTexture(const std::string &filename);
    void SetTexture(const SDL_Surface*);

    const SDL_Surface* GetTexture() const
    {
        return mTexture;
    }

protected:
    glm::mat4 mModel;

    GLuint mAttribSize = 0;
    GLuint mAttribCount = 0;
    GLfloat *mAttribData;

    GLuint mIndexSize = 0;
    GLuint *mIndexData;

    GLuint mProgramID;
    GLuint mWireProgramID;
    GLBufferInfo mGLInfo;

    SDL_Surface *mTexture;

    Renderer* mRenderer;

private:
    /**
     * Called by Renderer when this object is added to the rendered world.
     * Should allocate OpenGL buffers and textures.
     */
    virtual void BeforeRender();

    /**
     * Called by Renderer when this object is removed from rendered world.
     * Should deallocate any OpenGL buffers and textures.
     */
    virtual void AfterRender();

    /**
     * Handles actual rendering of this object (called from Renderer::render())
     */
    virtual void Render(glm::mat4 projection, glm::mat4 view, const Renderer* renderer);
};

}
