#include "Common.hpp"

#include "Widget.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"

using vh::App;
using vh::Renderer2D;

gui::Widget::~Widget()
{
    OnDestroy(this);

    for (Widget* child : mChildren)
    {
        if (child) delete child;
    }

    if (mBgImage != nullptr)
    {
        SDL_FreeSurface(mBgImage);
        mBgImage = nullptr;
    }
}

void gui::Widget::SetPos(int32_t x, int32_t y, eAnchor::Type anchor /*= eAnchor::TopLeft*/)
{
    mPosX = x;
    mPosY = y;
    mAnchor = anchor;
    SetDirty();
}

void gui::Widget::GetSize(int32_t& width, int32_t& height) const
{
    width = mWidth;
    height = mHeight;
}

void gui::Widget::SetSize(int32_t width, int32_t height)
{
    if (mWidth != width || mHeight != height) SetDirty();
    mWidth = width;
    mHeight = height;
}

void gui::Widget::SetChildDirty()
{
    mChildDirty = true;
    if (mParent != nullptr) mParent->SetChildDirty();
}

void gui::Widget::Draw(int32_t x, int32_t y)
{
    // supposed to be implemented in child classes
}

void gui::Widget::AddChild(Widget* widget)
{
    widget->mParent = this;
    mChildren.push_back(widget);
}

bool gui::Widget::IsPointInside(int32_t x, int32_t y)
{
    return x >= mAbsPosX && x <= mAbsPosX + mWidth &&
        y >= mAbsPosY && y <= mAbsPosY + mHeight;
}

void gui::Widget::OnClick(int32_t x, int32_t y)
{
    // default behavior: propagate event to children
    for (Widget* child : mChildren)
    {
        if (child->IsPointInside(x, y))
        {
            child->OnClick(x, y);
        }
    }
}

void gui::Widget::SetDirty()
{
    mDirty = true;
    SetChildDirty();
}

void gui::Widget::SetBackground(const vh::Color& color)
{
    mBgColor = color;
    SetDirty();
}

void gui::Widget::SetBackground(const char* imagePath)
{
    SDL_Surface* surf = IMG_Load(imagePath);
    if (surf != nullptr)
    {
        if (mBgImage) SDL_FreeSurface(mBgImage);
        mBgImage = surf;
        SetDirty();
    }
    else
    {
        LOG(ERROR) << "Image load error: " << IMG_GetError();
    }
}

void gui::Widget::Draw(Widget* parent, bool force /* = false*/)
{
    if (mDirty || force)
    {
        /* run some custom size update logic, if any */
        UpdateSize();

        /* calculate screen-relative coordinates */
        CalcAbsPos(parent);

        /* draw background */
        Renderer2D* render = vh::App::Get<Renderer2D>();
        CHECK(render);
        render->FillRect(mAbsPosX, mAbsPosY, mWidth, mHeight, mBgColor);

        if (mBgImage != nullptr)
        {
            render->DrawImage(mBgImage, mAbsPosX, mAbsPosY, mWidth, mHeight);
        }

        LOG(INFO) << "Draw: " << mAbsPosX << " " << mAbsPosY;
        /* draw this widget */
        Draw(mAbsPosX, mAbsPosY);

        /* set to also redraw all children */
        mChildDirty = true;
        force = true;

        /* reset dirty flag */
        mDirty = false;
    }

    /* draw all children */
    if (mChildDirty)
    {
        for (Widget* child : mChildren)
        {
            child->Draw(this, force);
        }

        mChildDirty = false;
    }
}

void gui::Widget::CalcAbsPos(Widget* parent)
{
    /* base point */
    int32_t posX = 0;
    int32_t posY = 0;

    if (parent)
    {
        /* get base point from parent */
        if (mPosX == ePos::Center) posX = parent->mAbsPosX + parent->mWidth / 2;
        else if (mPosX == ePos::Right) posX = parent->mAbsPosX + parent->mWidth;
        else posX = parent->mAbsPosX;

        if (mPosY == ePos::Center) posY = parent->mAbsPosY + parent->mHeight / 2;
        else if (mPosY == ePos::Bottom) posY = parent->mAbsPosY + parent->mHeight;
        else posY = parent->mAbsPosY;
    }

    int32_t relPosX = mPosX < 0 ? 0 : mPosX;
    int32_t relPosY = mPosY < 0 ? 0 : mPosY;

    /* calc screen-relative coordinates based on anchor position */
    switch (mAnchor)
    {
    case gui::eAnchor::TopLeft:
        mAbsPosX = posX + relPosX;
        mAbsPosY = posY + relPosY;
        break;
    case gui::eAnchor::TopCenter:
        mAbsPosX = posX + relPosX - mWidth / 2;
        mAbsPosY = posY + relPosY;
        break;
    case gui::eAnchor::TopRight:
        mAbsPosX = posX + relPosX - mWidth;
        mAbsPosY = posY + relPosY;
        break;
    case gui::eAnchor::CenterLeft:
        mAbsPosX = posX + relPosX;
        mAbsPosY = posY + relPosY - mHeight / 2;
        break;
    case gui::eAnchor::CenterCenter:
        mAbsPosX = posX + relPosX - mWidth / 2;
        mAbsPosY = posY + relPosY - mHeight / 2;
        break;
    case gui::eAnchor::CenterRight:
        mAbsPosX = posX + relPosX - mWidth;
        mAbsPosY = posY + relPosY - mHeight / 2;
        break;
    case gui::eAnchor::BottomLeft:
        mAbsPosX = posX + relPosX;
        mAbsPosY = posY + relPosY - mHeight;
        break;
    case gui::eAnchor::BottomCenter:
        mAbsPosX = posX + relPosX - mWidth / 2;
        mAbsPosY = posY + relPosY - mHeight;
        break;
    case gui::eAnchor::BottomRight:
        mAbsPosX = posX + relPosX - mWidth;
        mAbsPosY = posY + relPosY - mHeight;
        break;
    default:
        mAbsPosX = posX + relPosX;
        mAbsPosY = posY + relPosY;
        break;
    }
}
