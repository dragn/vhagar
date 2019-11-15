#include "Modules/VhModules_PCH.hpp"
#include "TextOverlay.hpp"

#include "Modules/Renderer3D/RenderableBehavior/BOverlay.hpp"
#include "Modules/ResourceSystem/ResourceTypes/RFont.hpp"

vh::TextOverlay::TextOverlay(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    : mPosX(x)
    , mPosY(y)
    , mWidth(width)
    , mHeight(height)
{
}

void vh::TextOverlay::Create()
{
    mActor = App::Get<MWorld>()->CreateActor("TextOverlay");
    mActor.lock()->AddBehavior<BOverlay>(V2(mPosX, mPosY), V2(mWidth, mHeight));
    mActor.lock()->StartPlay();

    mFont = App::Get<MResourceSystem>()->GetResource<RFont>("Fonts/Roboto-Regular.ttf")->GetFont(16);
    CHECK(mFont) << "Could not open font";
}

void vh::TextOverlay::Destroy()
{
    if (!mActor.expired())
    {
        App::Get<MWorld>()->DestroyActor(mActor.lock()->GetID());
    }
}

void vh::TextOverlay::SetText(const char* text)
{
    if (!mActor.expired() && mFont)
    {
        BOverlay* overlay = mActor.lock()->GetBehaviorOfType<BOverlay>();
        CHECK(overlay);

        SDL_Surface* surf = overlay->GetSurf();
        CHECK(surf);

        SDL_Color fg = { 255, 255, 255, 255 };

        SDL_Surface* temp;
        SDL_FillRect(surf, NULL, 0x00);
        temp = TTF_RenderText_Blended(mFont, text, fg);
        SDL_BlitSurface(temp, NULL, surf, NULL);
        SDL_FreeSurface(temp);

        overlay->Update();
    }
}
