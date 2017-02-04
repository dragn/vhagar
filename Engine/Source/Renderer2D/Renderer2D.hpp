#pragma once

#include "App/Component.hpp"
#include "Color.hpp"

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

class Renderer2D : public Component
{
    COMPONENT_NAME(Renderer2D);

public:
    Renderer2D();
    Renderer2D(const Renderer2DOptions& options);

    /* vh::Component interface */
    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

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

    void DrawText(TTF_Font* font, const char* text, int32_t x, int32_t y);

    void DrawImage(int32_t x, int32_t y, int32_t width, int32_t height, SDL_Surface* surf);

private:
    Renderer2DOptions mOptions;
    SDL_Window *mWindow;
    SDL_Renderer* mRenderer;
};

}