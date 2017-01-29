#pragma once

#include <list>

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

private:
    void Draw(Widget* parent);

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

    std::list<Widget*> mChildren;
};

}
