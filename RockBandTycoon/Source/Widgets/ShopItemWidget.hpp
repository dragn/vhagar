#pragma once

#include "GUI2D/Widget.hpp"
#include "Components/Resources.hpp"
#include "Utils/Delegate.hpp"

class ShopItemWidget : public gui::Widget
{
public:
    ShopItemWidget(const ItemRes& item);

    virtual void Draw(int32_t x, int32_t y) override;

    const ItemRes& GetItem() const { return mItem; }

    vh::MultiDelegate<const ItemRes&> OnBuy;

protected:
    virtual void OnClickInternal(int32_t x, int32_t y) override;
    virtual bool IsFocusable() const override { return true; }
    virtual SDL_Cursor* GetCursor();

private:
    ItemRes mItem;
    SDL_Surface* mItemImg;
};
