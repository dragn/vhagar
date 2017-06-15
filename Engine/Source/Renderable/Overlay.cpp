#include "Vhagar.hpp"

using namespace vh;

void Overlay::SetTexture(SDL_Surface *s)
{
    // TODO optimize performance (use PBO? or FBO?)
    glDeleteTextures(1, &texId);

    texId = Utils::BufferTexture2D(s);

    _UpdateVertices(s->w, s->h);
}

void Overlay::Init()
{
    programID = Utils::GetShaderProgram("OSD");
    if (programID < 0)
    {
        LOG(INFO) << "Unable to load program OSD";
        return;
    }
}

void Overlay::Destroy()
{
    glDeleteTextures(1, &texId);
    texId = 0;
    glDeleteBuffers(1, &vertexBuffer);
    vertexBuffer = 0;
}

void Overlay::_UpdateVertices(uint32_t w, uint32_t h)
{
    Renderer* render = App::Get<Renderer>();

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
    std::copy(v, v + 12, vertices);
    mBounds = V4(glX, glY, w / width, h / height);

    // -- update vertex buffer
    glDeleteBuffers(1, &vertexBuffer);
    vertexBuffer = Utils::BufferData(12, vertices);
}

void Overlay::Render()
{
    glDisable(GL_CULL_FACE);
    glUseProgram(programID);

    Utils::PutUniformVec4(programID, "uBounds", mBounds);

    if (texId > 0) glBindTexture(GL_TEXTURE_2D, texId);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);

    glEnable(GL_CULL_FACE);
}

vh::Overlay::~Overlay()
{
    Destroy();
}

void vh::Overlay::SetPos(uint32_t x, uint32_t y)
{
    mPosX = x;
    mPosY = y;
}
