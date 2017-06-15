#pragma once

#include "Renderable/Overlay.hpp"
#include "SDL_ttf.h"

namespace vh
{

class TextOverlay
{
public:
    TextOverlay(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    void SetSize(uint32_t width, uint32_t height);

    void Create();
    void Destroy();

    void SetText(const char* text);
    void Render();

private:
    uint32_t mPosX;
    uint32_t mPosY;
    TTF_Font* mFont;
    SDL_Surface* mSurf;
    Overlay mOverlay;
};

} // namespace vh
