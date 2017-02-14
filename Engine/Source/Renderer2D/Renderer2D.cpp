#include "Common.hpp"

#include "Renderer2D.hpp"

vh::Renderer2D::Renderer2D()
    : Component(eTickFrequency::NORMAL)
    , mWindow(nullptr)
{
}

vh::Renderer2D::Renderer2D(const Renderer2DOptions& options)
    : Component(eTickFrequency::NORMAL)
    , mOptions(options)
    , mWindow(nullptr)
{
}

void vh::Renderer2D::TickInit(uint32_t delta)
{
    LOG(INFO) << "SDL Initialization";
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        LOG(FATAL) << SDL_GetError();
        Close();
        return;
    }

    int imgInitFlags = IMG_INIT_PNG;
    if ((IMG_Init(imgInitFlags) & imgInitFlags) != imgInitFlags)
    {
        LOG(FATAL) << "Failed to init SDL_Image: " << IMG_GetError();
        Close();
    }

    Uint32 flags = 0;

    if (mOptions.fullscreen == Renderer2DOptions::FS_Borderless) flags |= SDL_WINDOW_BORDERLESS;
    if (mOptions.fullscreen == Renderer2DOptions::FS_Fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

    mWindow = SDL_CreateWindow(
        "Rock Band Tycoon",
        SDL_WINDOWPOS_CENTERED_DISPLAY(mOptions.monitor),
        SDL_WINDOWPOS_CENTERED_DISPLAY(mOptions.monitor),
        mOptions.screenWidth,
        mOptions.screenHeight,
        flags);

    if (mWindow == nullptr)
    {
        LOG(FATAL) << "SDL create windows error: " << SDL_GetError();
        Close();
        return;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

    if (mRenderer == nullptr)
    {
        LOG(FATAL) << "SDL create renderer error: " << SDL_GetError();
        Close();
        return;
    }

    SDL_RenderSetIntegerScale(mRenderer, SDL_TRUE);
    SDL_RenderSetScale(mRenderer, mOptions.scale, mOptions.scale);
    SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);

    FinishInit();
}

void vh::Renderer2D::TickRun(uint32_t delta)
{
    CHECK(mRenderer);

    SDL_RenderPresent(mRenderer);
}

void vh::Renderer2D::TickClose(uint32_t delta)
{
    if (mRenderer) SDL_DestroyRenderer(mRenderer);
    if (mWindow) SDL_DestroyWindow(mWindow);
    IMG_Quit();
    SDL_Quit();

    FinishClose();
}

void vh::Renderer2D::DrawRect(int32_t x, int32_t y, int32_t width, int32_t height, Color color)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;

    SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(mRenderer, &rect);
}

void vh::Renderer2D::FillRect(int32_t x, int32_t y, int32_t width, int32_t height, Color color)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;

    SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(mRenderer, &rect);
}

void vh::Renderer2D::CalcTextSize(TTF_Font* font, const char* text, int32_t& outWidth, int32_t& outHeight)
{
    CHECK(font);
    CHECK(text);

    SDL_Color any = { 0,0,0,0 };

    SDL_Surface* surf = TTF_RenderText_Solid(font, text, any);
    if (surf == nullptr)
    {
        outWidth = 0;
        outHeight = 0;
        return;
    }

    outWidth = surf->w;
    outHeight = surf->h;

    SDL_FreeSurface(surf);
}

void vh::Renderer2D::DrawText(TTF_Font* font, const char* text, Color clr, int32_t x, int32_t y, int32_t maxW /* = 0*/)
{
    CHECK(font);
    CHECK(text);

    SDL_Color white{ clr.r, clr.g, clr.b, clr.a };
    SDL_Surface* surf = TTF_RenderText_Solid(font, text, white);
    if (surf == nullptr)
    {
        return;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surf);
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = surf->w;
    dst.h = surf->h;

    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    src.w = surf->w;
    src.h = surf->h;

    if (maxW && src.w > maxW)
    {
        src.w = maxW;
        src.x = surf->w - maxW;
        dst.w = maxW;
    }

    SDL_RenderCopy(mRenderer, tex, &src, &dst);

    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}

void vh::Renderer2D::DrawImage(SDL_Surface* surf, int32_t x, int32_t y, int32_t w, int32_t h)
{
    CHECK(surf);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surf);
    if (!tex)
    {
        LOG(ERROR) << "Could not create texture: " << SDL_GetError();
        return;
    }

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(mRenderer, tex, NULL, &dst);

    SDL_DestroyTexture(tex);
}
