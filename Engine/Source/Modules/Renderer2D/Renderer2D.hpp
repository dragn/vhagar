#pragma once

#include "Core/App/Module.hpp"
#include "Color.hpp"

#undef DrawText

namespace vh
{

/**
 * Renderer options
 */
struct Renderer2DOptions
{
    uint32_t screenWidth = 1024;
    uint32_t screenHeight = 768;
    uint8_t scale = 1;

    enum
    {
        FS_Window = 0,
        FS_Fullscreen,
        FS_Borderless
    } fullscreen = FS_Window;

    enum
    {
        MON_First = 0,
        MON_Second
    } monitor = MON_First;
};

class Renderer2D : public Module
{
    VH_MODULE(Renderer2D);

public:
    Renderer2D();
    Renderer2D(const Renderer2DOptions& options);

    /* vh::Module interface */
    virtual Ret TickInit(uint32_t delta) override;
    virtual Ret TickClose(uint32_t delta) override;

    virtual void StartFrame() override;
    virtual void EndFrame() override;

    uint32_t GetWidth() const
    {
        return mOptions.screenWidth / mOptions.scale;
    }

    uint32_t GetHeight() const
    {
        return mOptions.screenHeight / mOptions.scale;
    }

    uint32_t GetScale() const { return mOptions.scale; }

    void DrawRect(int32_t x, int32_t y, int32_t width, int32_t height, Color color);
    void FillRect(int32_t x, int32_t y, int32_t width, int32_t height, Color color);

    void CalcTextSize(TTF_Font* font, const char* text, int32_t& outWidth, int32_t& outHeight, int32_t wrap = 0);
    void DrawText(TTF_Font* font, const char* text, Color color, int32_t x, int32_t y, int32_t maxW = 0, bool wrap = false);

    void DrawImage(SDL_Surface* surf, int32_t x, int32_t y, int32_t width = 0, int32_t height = 0);
    void DrawImage(SDL_Surface* surf, int32_t x, int32_t y, float scale);
    void DrawImage(SDL_Surface* surf, SDL_Rect* src, SDL_Rect* dst);

    void SetAlpha(Uint8 alpha) { mAlpha = alpha; }

private:
    Renderer2DOptions mOptions;
    SDL_Window *mWindow;
    SDL_Renderer* mRenderer;
    SDL_Texture* mFrameBuf;
    SDL_Rect mDstRect;
    Uint8 mAlpha;
};

}