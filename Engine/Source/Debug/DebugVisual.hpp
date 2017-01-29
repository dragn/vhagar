#pragma once

#include "Renderable/Renderable.hpp"

namespace vh
{

struct DebugLabel
{
    V3 pos;
    const char* text;
};

class DebugVisual : public Renderable
{
public:
    DebugVisual() : mGLBuf(0) {}

    std::vector<DebugLabel>& GetLabels() { return mLabels; };

private:
    std::vector<DebugLabel> mLabels;

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
