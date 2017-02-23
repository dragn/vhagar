#pragma once

#include "GUI2D/Widget.hpp"
#include "Components/Resources.hpp"

class ShopItemWidget : public gui::Widget
{
public:
    ShopItemWidget(const ItemRes& item);

    virtual void Draw(int32_t x, int32_t y) override;

    const ItemRes& GetItem() const { return mItem; }

protected:
    virtual bool IsFocusable() const override { return true; }

private:
    ItemRes mItem;
    SDL_Surface* mItemImg;
};
