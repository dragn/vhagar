#pragma once

namespace vh {

using Utils::CubeMap;

class SkyBox : public Renderable {

public:
    SkyBox(CubeMap cubeMap) : cubeMap(cubeMap) {};

private:

    CubeMap cubeMap;

    GLBufferInfo glInfo;
    GLuint skyTexture;
    GLuint programID;

    /**
     * Called by Renderer when this object is added to the rendered world.
     * Should alocate OpenGL buffers.
     */
    virtual void BeforeRender();

    /**
     * Called by Renderer when this object is removed from rendered world.
     * Should deallocate any OpenGL buffers.
     */
    virtual void AfterRender();

    /**
     * Handles actual rendering of this object (called from Renderer::render())
     */
    virtual void Render(glm::mat4 projection, glm::mat4 view, const Renderer* renderer);
};

} // namespace vh
