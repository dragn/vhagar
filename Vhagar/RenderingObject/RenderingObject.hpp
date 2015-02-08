#pragma once

#include "../Common.hpp"

namespace Vhagar {

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
  };

  class RenderingObject {

    friend class Renderer;

    protected:
    /**
     * Is this object ready to render?
     * Should be set to true by beforeRender() on successful completion.
     */
    bool isReadyToRender = false;

    private:
    /**
     * Called by Renderer when this object is added to the rendered world.
     * Should alocate OpenGL buffers.
     */
    virtual void beforeRender() = 0;

    /**
     * Called by Renderer when this object is removed from rendered world.
     * Should deallocate any OpenGL buffers.
     */
    virtual void afterRender() = 0;

    /**
     * Handles actual rendering of this object (called from Renderer::render())
     *
     * FIXME passing View and Projection here means every object will have the same view. but ok for now...
     */
    virtual void render(glm::mat4 projection, glm::mat4 view) = 0;
  };

}
