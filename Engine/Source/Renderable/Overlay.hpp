#pragma once

#include "Renderable.hpp"

namespace vh {

class Overlay {

public:
    ~Overlay();

    void SetPos(uint32_t x, uint32_t y);

    void SetTexture(SDL_Surface *surf);

    void Init();
    void Destroy();

    virtual void Render(const Renderer* renderer);

private:
    uint32_t mPosX = 0;
    uint32_t mPosY = 0;

    GLuint vertexBuffer = 0;
    GLuint programID = 0;

    GLfloat vertices[18];
    V4 mBounds;
    GLuint texId = 0;

    void _UpdateVertices(uint32_t w, uint32_t h);
};

} // namespace vh
