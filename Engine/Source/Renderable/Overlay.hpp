#pragma once

#include "Renderable.hpp"

namespace vh {

class Overlay : public Renderable {

public:
    Overlay();

    void SetBounds(Rect rect);

    void SetTexture(SDL_Surface *surf);

    /* Object methods */
    virtual void BeforeRender();
    virtual void AfterRender();
    virtual void Render(glm::mat4 projection, glm::mat4 view);
    virtual void Render(glm::mat4 projection, glm::mat4 view, Light lightSource) {}; // empty
    /* end of Object methods */

private:

    GLuint vertexBuffer = 0;
    GLuint programID = 0;

    GLfloat vertices[18];
    V4 bounds;
    GLuint texId = 0;
};

} // namespace vh
