#include "Common.hpp"
#include "TextOverlay.hpp"
#include "App/App.hpp"
#include "Renderer/Renderer.hpp"

vh::TextOverlay::TextOverlay(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    : mPosX(x)
    , mPosY(y)
    , mSurf(nullptr)
{
    SetSize(width, height);
}

void vh::TextOverlay::SetSize(uint32_t width, uint32_t height)
{
    if (mSurf)
    {
        SDL_FreeSurface(mSurf);
    }

    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;

    mSurf = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
}

void vh::TextOverlay::Create()
{
    mOverlay.Init();
    mFont = TTF_OpenFont("Assets/Fonts/Roboto-regular.ttf", 12);
    if (!mFont)
    {
        LOG(ERROR) << "Could not open font";
    }
    SetText("");
}

void vh::TextOverlay::Destroy()
{
    mOverlay.Destroy();
    SDL_FreeSurface(mSurf);
    mSurf = nullptr;
}

void vh::TextOverlay::SetText(const char* text)
{
    if (mFont)
    {
        SDL_Color fg = { 255, 255, 255, 255 };

        SDL_Surface* temp;
        SDL_FillRect(mSurf, NULL, 0x00);
        temp = TTF_RenderText_Blended(mFont, text, fg);
        SDL_BlitSurface(temp, NULL, mSurf, NULL);
        SDL_FreeSurface(temp);
    }

    mOverlay.SetTexture(mSurf);
}

void vh::TextOverlay::Render()
{
    mOverlay.Render();
}
