#pragma once

#include "Renderable.hpp"

namespace vh {

class Overlay : public Renderable {

public:
    Overlay();

    void SetPos(uint32_t x, uint32_t y);

    void SetTexture(SDL_Surface *surf);

    /* Object methods */
    virtual void BeforeRender();
    virtual void AfterRender();
    virtual void Render(glm::mat4 projection, glm::mat4 view);
    virtual void Render(glm::mat4 projection, glm::mat4 view, const Light* lightSource) {}; // empty
    /* end of Object methods */

private:
    uint32_t mPosX;
    uint32_t mPosY;

    GLuint vertexBuffer = 0;
    GLuint programID = 0;

    GLfloat vertices[18];
    V4 mBounds;
    GLuint texId = 0;

    void _UpdateVertices(uint32_t w, uint32_t h);
};

} // namespace vh
