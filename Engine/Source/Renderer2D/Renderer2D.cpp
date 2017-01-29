#include "Common.hpp"

#include "Renderer2D.hpp"

vh::Renderer2D::Renderer2D()
    : Component(eTickFrequency::NORMAL)
    , mScale(4)
    , mWindow(nullptr)
{
}

vh::Renderer2D::Renderer2D(const Renderer2DOptions& options)
    : Component(eTickFrequency::NORMAL)
    , mScale(4)
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
    SDL_RenderSetScale(mRenderer, mScale, mScale);

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

void vh::Renderer2D::DrawText(TTF_Font* font, const char* text, int32_t x, int32_t y)
{
    CHECK(font);
    CHECK(text);

    SDL_Color white{ 0xff, 0xff, 0xff, 0xff };
    SDL_Surface* surf = TTF_RenderText_Blended(font, text, white);
    CHECK(surf) << "Text render error";

    SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surf);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surf->w;
    rect.h = surf->h;
    SDL_RenderCopy(mRenderer, tex, NULL, &rect);

    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}
