#pragma once

#include <list>
#include "Renderer2D/Color.hpp"

namespace gui
{

namespace eAnchor
{
enum Type
{
    TopLeft = 0,
    TopCenter,
    TopRight,
    CenterLeft,
    CenterCenter,
    CenterRight,
    BottomLeft,
    BottomCenter,
    BottomRight
};
}

namespace ePos
{
enum Type
{
    Left = -1,
    Center = -2,
    Right = -3,
    Top = -4,
    Bottom = -5
};
};

class Widget
{
    friend class View;

public:
    Widget()
        : mAnchor(eAnchor::TopLeft)
        , mPosX(0)
        , mPosY(0)
        , mWidth(0)
        , mHeight(0)
        , mDirty(true)
        , mChildDirty(true)
        , mParent(nullptr)
        , mBgColor()
        , mBgImage(nullptr)
    {
    }

    virtual ~Widget();

    void SetPos(int32_t x, int32_t y, eAnchor::Type anchor = eAnchor::TopLeft);
    void SetSize(int32_t width, int32_t height);
    void GetSize(int32_t& width, int32_t& height) const;

    virtual void Draw(int32_t x, int32_t y);
    void AddChild(Widget* widget);

    bool IsPointInside(int32_t x, int32_t y);

    virtual void OnClick(int32_t x, int32_t y);

    void SetDirty();

    void SetBackground(const vh::Color& color);
    void SetBackground(const char* imagePath);

    virtual void OnFocus() {};
    virtual void OnBlur() {};

    /* handle event passed to focused widget */
    virtual void HandleEvent(SDL_Event* event) {};

protected:
    virtual void UpdateSize() {};

private:
    void SetChildDirty();

    void Draw(Widget* parent);

    /* calculate screen-relative position of the widget */
    void CalcAbsPos(Widget* parent);

    eAnchor::Type mAnchor;

    /* parent-relative position */
    int32_t mPosX;
    int32_t mPosY;
    int32_t mWidth;
    int32_t mHeight;

    /* screen-relative position */
    int32_t mAbsPosX;
    int32_t mAbsPosY;

    bool mDirty;
    bool mChildDirty;

    std::list<Widget*> mChildren;
    Widget* mParent;

    vh::Color mBgColor;
    SDL_Surface* mBgImage;
};

}
