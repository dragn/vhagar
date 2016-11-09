#pragma once

#include "Renderer/Light.hpp"
#include "Renderable.hpp"

namespace vh {

class Mesh : public Renderable {

    friend class SkyBox;

public:
    Mesh() : texture(nullptr) {};

    virtual ~Mesh();

    /**
     * Sets Model matrix for this object
     */
    void setModel(glm::mat4 _model) { model = _model; }

    void setAttribData(GLuint size, GLuint count, GLfloat* data) {
        attribSize = size;
        attribCount = count;
        attribData = data;
    }

    void setIndexData(GLuint size, GLuint* data) {
        indexSize = size;
        indexData = data;
    }

    void setTexture(const std::string &filename);

protected:
    // FIXME fix inconsistent private member naming

    glm::mat4 model;

    GLuint attribSize = 0;
    GLuint attribCount = 0;
    GLfloat *attribData;

    GLuint indexSize = 0;
    GLuint *indexData;

    GLuint programID;
    GLBufferInfo glInfo;

    SDL_Surface *texture;

private:
    /**
     * Called by Renderer when this object is added to the rendered world.
     * Should alocate OpenGL buffers and textures.
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
    virtual void Render(glm::mat4 projection, glm::mat4 view, Light lightSource);

    virtual void Render(glm::mat4 projection, glm::mat4 view) {}; // Empty - render nothing without light (yet)
};

}
