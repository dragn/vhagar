#pragma once

#include "Renderable/Renderable.hpp"

namespace vh
{

class DebugVisual : public Renderable
{
public:
    DebugVisual() : mGLBuf(0) {}

    std::vector<V3>& GetPos() { return mPos; };

private:
    std::vector<V3> mPos;

    GLuint mProgramId;
    GLuint mGLBuf;

    GLint uidColor;
    GLuint uidMVP;

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

} // namespace vh
