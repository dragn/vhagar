#include "PCH.hpp"

#include "ShopItemWidget.hpp"
#include "Components/Resources.hpp"

using namespace vh;
using namespace gui;

ShopItemWidget::ShopItemWidget(const ShopItem& item)
    : mItem(item)
{
    SetSize(347, 65);
    SetBackground(Colors::White);

    Resources* res = App::Get<Resources>();
    mItemImg = res->GetItemImg(item.GetImg());

    item.GetImg();
}

void ShopItemWidget::Draw(int32_t x, int32_t y)
{
    Renderer2D* render = App::Get<Renderer2D>();
    GUI2D* gui = App::Get<GUI2D>();

    render->FillRect(x + 5, y + 6, 71, 53, vh::Color(0x68));

    SDL_Rect src;
    src.x = 7;
    src.y = 0;
    src.w = 57;
    src.h = 53;

    SDL_Rect dst;
    dst.x = x + 5 + 53 / 2 - mItemImg->w / 2;
    dst.y = y + 6 + 57 / 2 - mItemImg->h / 2;
    dst.w = mItemImg->w;
    dst.h = mItemImg->h;

    render->DrawImage(mItemImg, nullptr, &dst);

    render->DrawText(gui->GetFont(), mItem.GetName().c_str(), vh::Color(0x00), x + 80, y + 6);

    if (IsFocused())
    {
        int32_t w, h;
        GetSize(w, h);
        render->DrawRect(x, y, w, h, Colors::Orange);
    }
}

void ShopItemWidget::OnClickInternal(int32_t x, int32_t y)
{
    if (IsFocused()) OnBuy(mItem);

    Widget::OnClickInternal(x, y);
}

SDL_Cursor* ShopItemWidget::GetCursor()
{
    if (mCursor != nullptr) return mCursor;
    gui::GUI2D* gui = vh::App::Get<gui::GUI2D>();
    CHECK(gui);
    mCursor = gui->GetHandCursor();
    return mCursor;
}
