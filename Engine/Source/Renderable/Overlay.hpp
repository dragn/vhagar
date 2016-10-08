#pragma once

namespace vh {

class Overlay : public Renderable {

public:

    void setBounds(Rect rect);

    void setTexture(SDL_Surface *surf);

    /* Object methods */
    virtual void beforeRender();
    virtual void afterRender();
    virtual void render(glm::mat4 projection, glm::mat4 view);
    virtual void render(glm::mat4 projection, glm::mat4 view, LightSource lightSource) {}; // empty
    /* end of Object methods */

private:

    GLuint vertexBuffer = 0;
    GLuint programID = 0;

    GLfloat vertices[18];
    V4 bounds;
    SDL_Surface *surf = NULL;
    GLuint texId = 0;
};

} // namespace vh
