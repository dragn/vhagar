#pragma once

#include "GUI2D/Widget.hpp"
#include "Components/Resources.hpp"
#include "Utils/Delegate.hpp"
#include "Gameplay/ShopItem.hpp"

class ShopItemWidget : public gui::Widget
{
public:
    ShopItemWidget(const ShopItem& item);

    virtual void Draw(int32_t x, int32_t y) override;

    const ShopItem& GetItem() const { return mItem; }

    vh::MultiDelegate<const ShopItem&> OnBuy;

protected:
    virtual void OnFocusInternal();
    virtual void OnBlurInternal();

    virtual void OnClickInternal(int32_t x, int32_t y) override;
    virtual bool IsFocusable() const override { return true; }
    virtual SDL_Cursor* GetCursor();

private:
    ShopItem mItem;
    SDL_Surface* mItemImg;

    gui::ButtonWidget* mTryBtn;
    gui::ButtonWidget* mBuyBtn;
};
