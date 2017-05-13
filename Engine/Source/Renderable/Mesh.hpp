#pragma once

#include "Renderer/Lights.hpp"
#include "Renderable.hpp"

namespace vh {

class Mesh : public Renderable {

    friend class SkyBox;

public:
    Mesh();;

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

    void SetTexture(uint32_t* rgbaData, size_t width, size_t height);
    void GetTexture(uint32_t*& rgbaData, size_t& width, size_t& height) const;

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

    Renderer* mRenderer;

    uint32_t* mTexDta;
    size_t mTexW;
    size_t mTexH;

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

    // uniform locations
    GLint uidMVP;
    GLint uidM;
    GLint uidV;
    GLint uidPLightPos;
    GLint uidPLightInt;
    GLint uidPLightNum;

    // wireframe uniform
    GLuint uidWireColor;
    GLuint uidWireMVP;

    virtual void BufferTexture();
};

}
