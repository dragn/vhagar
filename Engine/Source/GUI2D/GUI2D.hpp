#pragma once

#include "App/Component.hpp"
#include "View.hpp"
#include "SDL_ttf.h"

namespace gui
{

class GUI2D : public vh::Component
{
    COMPONENT_NAME(GUI2D);

    const int FONT_SIZE = 14;

public:
    GUI2D()
        : vh::Component(vh::eTickFrequency::NORMAL)
        , mActiveView(nullptr)
        , mFont(nullptr)
    {}

    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    void SetView(View* view);

    virtual void HandleEvent(SDL_Event* event);

    TTF_Font* GetFont() const { return mFont; }

    /* Calculate text size rendered with default font */
    void CalcTextSize(const char* text, int32_t& outWidth, int32_t& outHeight);
    void DrawText(const char* text, int32_t x, int32_t y);

private:
    View* mActiveView;
    uint32_t mScale;

    TTF_Font* mFont;
};

}
