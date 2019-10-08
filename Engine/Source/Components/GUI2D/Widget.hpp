#pragma once

#include <list>
#include "Renderer2D/Color.hpp"
#include "Utils/Delegate.hpp"

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
    friend class GUI2D;

public:
    Widget();

    virtual ~Widget();

    void SetPos(int32_t x, int32_t y, eAnchor::Type anchor = eAnchor::TopLeft);
    void SetSize(int32_t width, int32_t height);
    void GetSize(int32_t& width, int32_t& height) const;

    virtual void Draw(int32_t x, int32_t y);
    void AddChild(Widget* widget);
    void RemoveChild(Widget* widget);

    bool IsPointInside(int32_t x, int32_t y);
    virtual void OnMouseMove(int32_t x, int32_t y);

    virtual void SetDirty();

    void SetBackground(const vh::Color& color);
    void SetBackground(const char* imagePath);

    /* handle event passed to focused widget */
    virtual void HandleEvent(SDL_Event* event) {};

    vh::MultiDelegate<Widget*> OnDestroy;
    vh::MultiDelegate<> OnClick;
    vh::MultiDelegate<> OnFocus;
    vh::MultiDelegate<> OnBlur;

    bool IsFocused() const { return mFocused; }

    bool IsVisible() const { return mVisible; }
    void SetVisible(bool visible);

protected:
    virtual void OnClickInternal(int32_t x, int32_t y);

    virtual bool IsFocusable() const { return false; }

    virtual void OnFocusInternal();
    virtual void OnBlurInternal();

    virtual void UpdateSize() {};

    virtual SDL_Cursor* GetCursor();
    SDL_Cursor* mCursor;

    Widget* mParent;

private:
    void SetChildDirty();

    void Draw(Widget* parent, bool force = false);

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

    vh::Color mBgColor;
    SDL_Surface* mBgImage;

    bool mFocused;
    bool mVisible;
};

}
