#include "Common.hpp"

#include "Renderer2D.hpp"

VH_COMPONENT_IMPL(vh::Renderer2D);

vh::Renderer2D::Renderer2D()
    : Component(eTickFrequency::NORMAL)
    , mWindow(nullptr)
    , mRenderer(nullptr)
    , mFrameBuf(nullptr)
    , mAlpha(SDL_ALPHA_OPAQUE)
{
}

vh::Renderer2D::Renderer2D(const Renderer2DOptions& options)
    : Renderer2D()
{
    mOptions = options;
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
    SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);

    mFrameBuf = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET, mOptions.screenWidth, mOptions.screenHeight);

    if (mFrameBuf == nullptr)
    {
        LOG(FATAL) << "Could not create frame buffer texture: " << SDL_GetError();
        Close();
        return;
    }

    mDstRect.x = 0;
    mDstRect.y = 0;
    mDstRect.w = mOptions.screenWidth * mOptions.scale;
    mDstRect.h = mOptions.screenHeight * mOptions.scale;

    FinishInit();
}

void vh::Renderer2D::TickClose(uint32_t delta)
{
    if (mRenderer) SDL_DestroyRenderer(mRenderer);
    if (mWindow) SDL_DestroyWindow(mWindow);
    IMG_Quit();
    SDL_Quit();

    FinishClose();
}

void vh::Renderer2D::StartFrame()
{
    SDL_SetRenderTarget(mRenderer, mFrameBuf);
}

void vh::Renderer2D::EndFrame()
{
    SDL_SetRenderTarget(mRenderer, nullptr);

    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0xff);
    SDL_RenderClear(mRenderer);

    SDL_SetTextureAlphaMod(mFrameBuf, mAlpha);
    SDL_SetTextureBlendMode(mFrameBuf, SDL_BLENDMODE_BLEND);

    SDL_RenderCopy(mRenderer, mFrameBuf, nullptr, &mDstRect);
    SDL_RenderPresent(mRenderer);
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

void vh::Renderer2D::CalcTextSize(TTF_Font* font, const char* text, int32_t& outWidth, int32_t& outHeight, int32_t wrap /* = 0 */)
{
    CHECK(font);
    CHECK(text);

    SDL_Color any = { 0,0,0,0 };

    SDL_Surface* surf =
        wrap > 0
        ? TTF_RenderText_Blended_Wrapped(font, text, any, wrap)
        : TTF_RenderText_Solid(font, text, any);

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

void vh::Renderer2D::DrawText(TTF_Font* font, const char* text, Color clr, int32_t x, int32_t y, int32_t maxW /* = 0*/, bool wrap /* = false */)
{
    CHECK(font);
    CHECK(text);

    SDL_Color white{ clr.r, clr.g, clr.b, clr.a };
    SDL_Surface* surf =
        wrap
        ? TTF_RenderText_Blended_Wrapped(font, text, white, maxW)
        : TTF_RenderText_Solid(font, text, white);
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

void vh::Renderer2D::DrawImage(SDL_Surface* surf, int32_t x, int32_t y, int32_t w /* = 0 */, int32_t h /* = 0 */)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w ? w : surf->w;
    dst.h = h ? h : surf->h;

    DrawImage(surf, NULL, &dst);
}

void vh::Renderer2D::DrawImage(SDL_Surface* surf, SDL_Rect* src, SDL_Rect* dst)
{
    CHECK(surf);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surf);
    if (!tex)
    {
        LOG(ERROR) << "Could not create texture: " << SDL_GetError();
        return;
    }

    SDL_RenderCopy(mRenderer, tex, src, dst);

    SDL_DestroyTexture(tex);
}

void vh::Renderer2D::DrawImage(SDL_Surface* surf, int32_t x, int32_t y, float scale)
{
    DrawImage(surf, x, y, surf->w * scale, surf->h * scale);
}
