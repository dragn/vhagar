#include "Modules/VhModules_PCH.hpp"
#include "TextOverlay.hpp"

#include "Modules/Renderer/MRenderer3D_Old.hpp"
#include "Modules/Renderer/RenderableBehavior/OverlayBehavior.hpp"
#include "Modules/ResourceSystem/ResourceTypes/RFont.hpp"

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
    mActor = App::Get<MWorld>()->CreateActor("TextOverlay");
    // mOverlay = mActor.lock()->AddBehavior<OverlayBehavior>()->Get();

    mActor.lock()->StartPlay();

    mFont = App::Get<MResourceSystem>()->GetResource<RFont>("Fonts/Roboto-Regular.ttf")->GetFont(16);
    CHECK(mFont) << "Could not open font";
}

void vh::TextOverlay::Destroy()
{
    if (mSurf != nullptr)
    {
        SDL_FreeSurface(mSurf);
        mSurf = nullptr;
    }
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

    if (mOverlay)
    {
        mOverlay->SetTexture(mSurf);
    }
}
