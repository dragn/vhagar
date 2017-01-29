#pragma once

#include "App/Component.hpp"

namespace vh
{

struct Color
{
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff)
        : r(r), g(g), b(b), a(a)
    {
    }

    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0xff;
};

/**
 * Renderer options
 */
struct Renderer2DOptions
{
    uint32_t screenWidth = 1024;
    uint32_t screenHeight = 768;

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
        return mOptions.screenWidth / mScale;
    }

    uint32_t GetHeight() const
    {
        return mOptions.screenHeight / mScale;
    }

    uint32_t GetScale() const { return mScale; }

    void DrawRect(int32_t x, int32_t y, int32_t width, int32_t height, Color color);

private:
    uint32_t mScale;
    Renderer2DOptions mOptions;
    SDL_Window *mWindow;
    SDL_Renderer* mRenderer;
};

}
