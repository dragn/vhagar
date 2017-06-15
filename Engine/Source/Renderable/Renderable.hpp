#pragma once

#include "Common.hpp"

namespace vh
{

class Renderer; // forward decl

struct GLBufferInfo
{
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

class Renderable
{
public:
    void AddUsage();
    void ReleaseUsage();

    bool IsLoaded() const { return mLoaded; }

protected:
    virtual bool DoLoad() = 0;
    virtual bool DoUnload() = 0;

private:
    int32_t id = -1;
    bool mLoaded = false;

    // Counts the rendered references.
    // Renderable is automatically buffered when the value changed from zero
    // and removed from render context, when value reaches zero again.
    uint32_t mRenderUsage = 0;
};

}
