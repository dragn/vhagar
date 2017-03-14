#include "PCH.hpp"

#include "HireWidget.hpp"
#include "Components/Resources.hpp"

using namespace gui;
using namespace vh;

HireWidget::HireWidget()
{
    GUI2D* gui = App::Get<GUI2D>();

    ButtonWidget* btn = new ButtonWidget("HIRE");
    btn->SetSize(46, 24);
    btn->SetPos(133, 44);
    btn->SetFont(gui->GetHdr2Font());
    btn->SetBackground(Colors::Orange);
    btn->OnClick.Add([this] () { OnHire(mHireItem); });
    AddChild(btn);
}

void HireWidget::Draw(int32_t x, int32_t y)
{
    vh::Renderer2D* render = vh::App::Get<vh::Renderer2D>();
    gui::GUI2D* gui = vh::App::Get<gui::GUI2D>();
    CHECK(render);
    CHECK(gui);

    SDL_Rect dst;
    dst.x = x + 8;
    dst.y = y + 16;
    dst.w = 56;
    dst.h = 56;

    SDL_Rect src;
    src.x = 15;
    src.y = 4;
    src.w = 28;
    src.h = 28;

    render->FillRect(x + 8, y + 16, 56, 56, vh::Color(0x88));

    if (mBodyImg != nullptr) render->DrawImage(mBodyImg, &src, &dst);
    if (mFaceImg != nullptr) render->DrawImage(mFaceImg, &src, &dst);
    if (mHairImg != nullptr) render->DrawImage(mHairImg, &src, &dst);

    render->DrawText(gui->GetFont(), mHireItem.GetName().c_str(), vh::Color(0x00), x + 68, y + 16);
}

void HireWidget::SetHireItem(const HireItem& item)
{
    Resources* res = App::Get<Resources>();

    mHairImg = res->GetHairImg(item.GetLooks().GetHair());
    mFaceImg = res->GetFaceImg(item.GetLooks().GetFace());
    mBodyImg = res->GetBodyImg(item.GetLooks().GetBody());

    mHireItem = item;
}

const HireItem& HireWidget::GetHireItem() const
{
    return mHireItem;
}
