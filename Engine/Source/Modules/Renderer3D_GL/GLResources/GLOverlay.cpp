#include "Modules/VhModules_PCH.hpp"
#include "GLOverlay.hpp"

#include "Modules/Renderer3D/MRenderer3D_Old.hpp"

using namespace vh;

void GLOverlay::BlitTexture(SDL_Surface* surf)
{
    CHECK(mTexId != 0);
    CHECK(surf);
    Utils::BufferTexture2D(mTexId, surf);
}

void GLOverlay::UpdateVertices()
{
    MRenderer3D* render = App::Get<MRenderer3D>();
    if (!render) return;

    CHECK(render);

    float width = 0.5f * static_cast<float>(render->GetOptions().screenWidth);
    float height = 0.5f * static_cast<float>(render->GetOptions().screenHeight);

    float glX = -1.0f + mPos.x / width;
    float glY = 1.0f - mPos.y / height;

    float w = mSize.x;
    float h = mSize.y;

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
    CHECK(mVertexBuffer == 0);
    mVertexBuffer = Utils::BufferData(12, mVertices);

    // -- create a texture
    CHECK(mTexId == 0);
    glGenTextures(1, &mTexId);
}

void vh::GLOverlay::SetupPayload(Payload& payload) const
{
    payload.progId = mShaderId;
    payload.texId = mTexId;
    payload.vertexBuffer = mVertexBuffer;
    payload.bounds = mBounds;
}

vh::GLOverlay::~GLOverlay()
{
    CHECK(mTexId == 0);
    CHECK(mVertexBuffer == 0);
}

bool vh::GLOverlay::DoLoad()
{
    mShaderId = Utils::GetShaderProgram("OSD");
    CHECK(mShaderId >= 0) << "Unable to load program OSD";

    UpdateVertices();

    return true;
}

bool vh::GLOverlay::DoUnload()
{
    glDeleteTextures(1, &mTexId);
    mTexId = 0;

    glDeleteBuffers(1, &mVertexBuffer);
    mVertexBuffer = 0;

    return true;
}
