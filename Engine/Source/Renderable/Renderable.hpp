#pragma once

#include "Common.hpp"

#include "../Renderer/PointLight.hpp"

namespace vh {

struct GLBufferInfo {
    /**
     * The number of vertex attribute arrays
     */
    GLuint attribCount = 0;

    /**
     * Vertex attributes buffer id and size
     */
    GLuint attribBuffer = 0;
    GLsizei attribBufferSize = 0;

    /**
     * Index array buffer id and size
     */
    GLuint indexBuffer = 0;
    GLsizei indexBufferSize = 0;

    /**
     *
     */
    GLuint texture = 0;
};

class Renderable {

public:
    friend class Renderer;
    virtual ~Renderable() {};

protected:
    /**
     * Is this object ready to render?
     * Should be set to true by beforeRender() on successful completion.
     */
    bool isReadyToRender = false;

    glm::mat4 MVP;

private:
    /**
     * Called by Renderer when this object is added to the rendered world.
     * Should alocate OpenGL buffers.
     */
    virtual void BeforeRender() = 0;

    /**
     * Called by Renderer when this object is removed from rendered world.
     * Should deallocate any OpenGL buffers.
     */
    virtual void AfterRender() = 0;

    /**
     * Handles actual rendering of this object (called from Renderer::render())
     * This variant does not accept light source and should only be used for object's, that do not use lights (like sky box).
     *
     * FIXME passing View and Projection here means every object will have the same view. but ok for now...
     */
    virtual void Render(glm::mat4 projection, glm::mat4 view, const Renderer* renderer) = 0;
};

}
