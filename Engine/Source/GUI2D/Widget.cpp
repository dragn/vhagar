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
    if (!mDirty) return;

    for (Widget* child : mChildren)
    {
        int32_t childX = 0, childY = 0;
        CalcChildPos(childX, childY, child);
        child->Draw(childX, childY);
    }

    mDirty = false;
}

void gui::Widget::AddChild(Widget* widget)
{
    mChildren.push_back(widget);
}

void gui::Widget::CalcChildPos(int32_t& x, int32_t& y, Widget* child)
{
    int32_t width, height;
    child->GetSize(width, height);

    int32_t posX = mPosX;
    int32_t posY = mPosY;

    if (child->mPosX == ePos::Center) posX = mPosX + mWidth / 2;
    else if (child->mPosX == ePos::Right) posX = mPosX + mWidth;

    if (child->mPosY == ePos::Center) posY = mPosY + mHeight / 2;
    else if (child->mPosY == ePos::Bottom) posY = mPosY + mHeight;

    switch (child->mAnchor)
    {
    case gui::eAnchor::TopLeft:
        x = posX + child->mPosX;
        y = posY + child->mPosY;
        break;
    case gui::eAnchor::TopCenter:
        x = posX + child->mPosX - width / 2;
        y = posY + child->mPosY;
        break;
    case gui::eAnchor::TopRight:
        x = posX + child->mPosX - width;
        y = posY + child->mPosY;
        break;
    case gui::eAnchor::CenterLeft:
        x = posX + child->mPosX;
        y = posY + child->mPosY - height / 2;
        break;
    case gui::eAnchor::CenterCenter:
        x = posX + child->mPosX - width / 2;
        y = posY + child->mPosY - height / 2;
        break;
    case gui::eAnchor::CenterRight:
        x = posX + child->mPosX - width;
        y = posY + child->mPosY - height / 2;
        break;
    case gui::eAnchor::BottomLeft:
        x = posX + child->mPosX;
        y = posY + child->mPosY - height;
        break;
    case gui::eAnchor::BottomCenter:
        x = posX + child->mPosX - width / 2;
        y = posY + child->mPosY - height;
        break;
    case gui::eAnchor::BottomRight:
        x = posX + child->mPosX - width;
        y = posY + child->mPosY - height;
        break;
    default:
        x = posX + child->mPosX;
        y = posY + child->mPosY;
        break;
    }
}
