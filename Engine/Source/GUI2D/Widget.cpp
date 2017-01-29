#include "Common.hpp"

#include "Widget.hpp"

gui::Widget::~Widget()
{
    for (Widget* child : mChildren)
    {
        if (child) delete child;
    }
}

void gui::Widget::SetPos(int32_t x, int32_t y, eAnchor::Type anchor /*= eAnchor::TopLeft*/)
{
    mPosX = x;
    mPosY = y;
    mAnchor = anchor;
    mDirty = true;
}

void gui::Widget::GetSize(int32_t& width, int32_t& height) const
{
    width = mWidth;
    height = mHeight;
}

void gui::Widget::SetSize(int32_t width, int32_t height)
{
    mWidth = width;
    mHeight = height;
    mDirty = true;
}

void gui::Widget::Draw(int32_t x, int32_t y)
{
    // supposed to be implemented in child classes
}

void gui::Widget::AddChild(Widget* widget)
{
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

void gui::Widget::Draw(Widget* parent)
{
    if (mDirty)
    {
        /* calculate screen-relative coordinates */
        CalcAbsPos(parent);

        /* draw this widget */
        Draw(mAbsPosX, mAbsPosY);

        /* draw all children */
        for (Widget* child : mChildren)
        {
            child->Draw(this);
        }

        /* reset dirty flag */
        mDirty = false;
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

    /* calc screen-relative coordinates based on anchor position */
    switch (mAnchor)
    {
    case gui::eAnchor::TopLeft:
        mAbsPosX = posX + mPosX;
        mAbsPosY = posY + mPosY;
        break;
    case gui::eAnchor::TopCenter:
        mAbsPosX = posX + mPosX - mWidth / 2;
        mAbsPosY = posY + mPosY;
        break;
    case gui::eAnchor::TopRight:
        mAbsPosX = posX + mPosX - mWidth;
        mAbsPosY = posY + mPosY;
        break;
    case gui::eAnchor::CenterLeft:
        mAbsPosX = posX + mPosX;
        mAbsPosY = posY + mPosY - mHeight / 2;
        break;
    case gui::eAnchor::CenterCenter:
        mAbsPosX = posX + mPosX - mWidth / 2;
        mAbsPosY = posY + mPosY - mHeight / 2;
        break;
    case gui::eAnchor::CenterRight:
        mAbsPosX = posX + mPosX - mWidth;
        mAbsPosY = posY + mPosY - mHeight / 2;
        break;
    case gui::eAnchor::BottomLeft:
        mAbsPosX = posX + mPosX;
        mAbsPosY = posY + mPosY - mHeight;
        break;
    case gui::eAnchor::BottomCenter:
        mAbsPosX = posX + mPosX - mWidth / 2;
        mAbsPosY = posY + mPosY - mHeight;
        break;
    case gui::eAnchor::BottomRight:
        mAbsPosX = posX + mPosX - mWidth;
        mAbsPosY = posY + mPosY - mHeight;
        break;
    default:
        mAbsPosX = posX + mPosX;
        mAbsPosY = posY + mPosY;
        break;
    }
}
