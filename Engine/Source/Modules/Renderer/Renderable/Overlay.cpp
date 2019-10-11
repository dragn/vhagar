#include "Modules/VhModules_PCH.hpp"
#include "Overlay.hpp"

#include "Modules/Renderer/MRenderer3D.hpp"

using namespace vh;

void Overlay::SetTexture(SDL_Surface *s)
{
    // TODO optimize performance (use PBO? or FBO?)
    glDeleteTextures(1, &mTexId);

    mTexId = Utils::BufferTexture2D(s);

    _UpdateVertices(s->w, s->h);
}

void Overlay::_UpdateVertices(uint32_t w, uint32_t h)
{
    MRenderer3D* render = App::Get<MRenderer3D>();
    if (!render) return;

    CHECK(render);

    float width = 0.5f * static_cast<float>(render->GetOptions().screenWidth);
    float height = 0.5f * static_cast<float>(render->GetOptions().screenHeight);

    float glX = -1.0f + mPosX / width;
    float glY = 1.0f - mPosY / height;

    GLfloat v[] = {
        glX,                glY,
        glX + w / width,    glY,
        glX + w / width,    glY - h / height,
        glX + w / width,    glY - h / height,
        glX,                glY - h / height,
        glX,                glY
    };
    std::copy(v, v + 12, mVertices);
    mBounds = V4(glX, glY, w / width, h / height);

    // -- update vertex buffer
    glDeleteBuffers(1, &mVertexBuffer);
    mVertexBuffer = Utils::BufferData(12, mVertices);
}

vh::Overlay::~Overlay()
{
    CHECK(mTexId == 0);
    CHECK(mVertexBuffer == 0);
}

void vh::Overlay::SetPos(uint32_t x, uint32_t y)
{
    mPosX = x;
    mPosY = y;
}

bool vh::Overlay::DoLoad()
{
    mShaderId = Utils::GetShaderProgram("OSD");
    CHECK(mShaderId >= 0) << "Unable to load program OSD";

    return true;
}

bool vh::Overlay::DoUnload()
{
    glDeleteTextures(1, &mTexId);
    mTexId = 0;
    glDeleteBuffers(1, &mVertexBuffer);
    mVertexBuffer = 0;

    return true;
}
