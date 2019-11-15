#pragma once

// #include "Modules/Renderer3D_GL/GLResources/Overlay.hpp"
#include "Modules/standard.hpp"

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

private:
    uint32_t mPosX;
    uint32_t mPosY;
    TTF_Font* mFont;
    SDL_Surface* mSurf;

    std::weak_ptr<Actor> mActor;
    // std::shared_ptr<Overlay> mOverlay;
};

} // namespace vh
