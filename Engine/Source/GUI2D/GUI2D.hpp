#pragma once

#include "App/Component.hpp"
#include "View.hpp"
#include "SDL_ttf.h"

namespace gui
{

struct GUI2DOptions
{
    const char* fontPath = "<font path is not set>";
    int fontSize = 0;
    const char* hdr1FontPath = "<font path is not set>";
    int hdr1FontSize = 0;
    const char* hdr2FontPath = "<font path is not set>";
    int hdr2FontSize = 0;
};

class GUI2D : public vh::Component
{
    COMPONENT_NAME(GUI2D);

public:
    GUI2D(const GUI2DOptions& opts)
        : vh::Component(vh::eTickFrequency::NORMAL)
        , mView(nullptr)
        , mModalView(nullptr)
        , mNextView(nullptr)
        , mNextModalView(nullptr)
        , mScale(1)
        , mFont(nullptr)
        , mOptions(opts)
        , mFocused(nullptr)
        , mArrowCursor(nullptr)
        , mBeamCursor(nullptr)
        , mHandCursor(nullptr)
        , mGoBack(false)
    {}

    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    void SetView(View* view);
    View* GetView() const { return mView; }

    void SetModalView(View* view);
    View* GetModalView() const { return mModalView; }

    void Back();

    virtual void HandleEvent(SDL_Event* event);

    TTF_Font* GetFont() const { return mFont; }
    TTF_Font* GetHdr1Font() const { return mHdr1Font; }
    TTF_Font* GetHdr2Font() const { return mHdr2Font; }

    /* Calculate text size rendered with default font */
    void CalcTextSize(const char* text, int32_t& outWidth, int32_t& outHeight);

    void SetFocus(Widget* widget);

    SDL_Cursor* GetArrowCursor();
    SDL_Cursor* GetHandCursor();
    SDL_Cursor* GetBeamCursor();

private:
    View* mView;
    View* mModalView;
    View* mNextView;
    View* mNextModalView;
    uint32_t mScale;

    TTF_Font* mFont;
    TTF_Font* mHdr1Font;
    TTF_Font* mHdr2Font;
    GUI2DOptions mOptions;

    Widget* mFocused;

    SDL_Cursor* mArrowCursor;
    SDL_Cursor* mBeamCursor;
    SDL_Cursor* mHandCursor;

    bool mGoBack;
};

}
