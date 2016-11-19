#include "Vhagar.hpp"

using namespace vh;

void Overlay::SetTexture(SDL_Surface *s)
{
    if (isReadyToRender)
    {
        if (texId > 0)
        {
            glDeleteTextures(1, &texId);
            texId = 0;
        }
        texId = Utils::BufferTexture2D(s);
    }

    _UpdateVertices(s->w, s->h);
}

void Overlay::BeforeRender()
{
    vertexBuffer = Utils::BufferData(12, vertices);

    programID = Utils::GetShaderProgram("OSD");
    if (programID < 0)
    {
        LOG(INFO) << "Unable to load program OSD";
        return;
    }

    isReadyToRender = true;
}

void Overlay::AfterRender()
{
    if (texId > 0)
    {
        glDeleteTextures(1, &texId);
        texId = 0;
    }
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

    if (isReadyToRender)
    {
        glDeleteBuffers(1, &vertexBuffer);
        vertexBuffer = Utils::BufferData(12, vertices);
    }
}

void Overlay::Render(glm::mat4 projection, glm::mat4 view, const Renderer* renderer)
{
    if (!isReadyToRender) return;

    glDisable(GL_CULL_FACE);
    glUseProgram(programID);

    Utils::PutUniformVec4(programID, "uBounds", mBounds);

    if (texId > 0) glBindTexture(GL_TEXTURE_2D, texId);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);

    glEnable(GL_CULL_FACE);
}

vh::Overlay::Overlay()
    : mPosX(0)
    , mPosY(0)
{
    _UpdateVertices(0, 0);
}

void vh::Overlay::SetPos(uint32_t x, uint32_t y)
{
    mPosX = x;
    mPosY = y;
}
