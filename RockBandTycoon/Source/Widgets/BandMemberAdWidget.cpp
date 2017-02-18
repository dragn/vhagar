#include "PCH.hpp"

#include "BandMemberAdWidget.hpp"

void BandMemberAdWidget::Draw(int32_t x, int32_t y)
{
    vh::Renderer2D* render = vh::App::Get<vh::Renderer2D>();
    gui::GUI2D* gui = vh::App::Get<gui::GUI2D>();
    CHECK(render);
    CHECK(gui);

    SDL_Rect dst;
    dst.x = x + 8;
    dst.y = y + 12;
    dst.w = 56;
    dst.h = 56;

    SDL_Rect src;
    src.x = 1;
    src.y = 4;
    src.w = 28;
    src.h = 28;

    render->FillRect(x + 8, y + 12, 56, 56, vh::Color(0x88));

    if (mBodyImg != nullptr) render->DrawImage(mBodyImg, &src, &dst);
    if (mFaceImg != nullptr) render->DrawImage(mFaceImg, &src, &dst);
    if (mHairImg != nullptr) render->DrawImage(mHairImg, &src, &dst);
    if (mHandsImg != nullptr) render->DrawImage(mHandsImg, &src, &dst);
}

void BandMemberAdWidget::UpdateSize()
{
    SetSize(186, 78);
}
