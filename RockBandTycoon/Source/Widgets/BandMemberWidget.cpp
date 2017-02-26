#include "PCH.hpp"
#include "BandMemberWidget.hpp"
#include "Components/Resources.hpp"

using namespace gui;

void BandMemberWidget::Draw(int32_t x, int32_t y)
{
    vh::Renderer2D* render = vh::App::Get<vh::Renderer2D>();
    GUI2D* gui = vh::App::Get<gui::GUI2D>();
    CHECK(render);
    CHECK(gui);

    if (mLegsImg != nullptr) render->DrawImage(mLegsImg, x, y, 2.0f);
    if (mBodyImg != nullptr) render->DrawImage(mBodyImg, x, y, 2.0f);
    if (mFaceImg != nullptr) render->DrawImage(mFaceImg, x, y, 2.0f);
    if (mHairImg != nullptr) render->DrawImage(mHairImg, x, y, 2.0f);

    int32_t itemX, itemY;
    GetOffset(itemX, itemY);
    if (mItemImg != nullptr) render->DrawImage(mItemImg, x + itemX, y + itemY, 2.0f);

    if (mHandsImg != nullptr) render->DrawImage(mHandsImg, x, y, 2.0f);

    int32_t w, h, textW, textH;
    GetSize(w, h);
    render->CalcTextSize(gui->GetFont(), mBandMember.GetName().c_str(), textW, textH);
    render->DrawText(gui->GetFont(), mBandMember.GetName().c_str(), vh::Color(0xff), x + w / 2 - textW / 2, y);
}

void BandMemberWidget::SetBandMember(const BandMember& member)
{
    // look out! it's a copy
    mBandMember = member;

    // loading images
    Resources* bp = vh::App::Get<Resources>();
    CHECK(bp);

    mHairImg = bp->GetHairImg(mBandMember.GetLooks().GetHair());
    mFaceImg = bp->GetFaceImg(mBandMember.GetLooks().GetFace());
    mBodyImg = bp->GetBodyImg(mBandMember.GetLooks().GetBody());
    mHandsImg = bp->GetHandsImg(mBandMember.GetLooks().GetHands());
    mLegsImg = bp->GetLegsImg(mBandMember.GetLooks().GetLegs());
    mItemImg = bp->GetItemImg(mBandMember.GetItem().GetImg());

    SetDirty();
}

void BandMemberWidget::UpdateSize()
{
    if (mHairImg)
    {
        SetSize(2 * mHairImg->w, 2 * mHairImg->h);
    }
}

const BandMember& BandMemberWidget::GetBandMember() const
{
    return mBandMember;
}

void BandMemberWidget::GetOffset(int32_t& x, int32_t& y)
{
    switch (mBandMember.GetType())
    {
    case eBandSlot::Guitar:
    case eBandSlot::Bass:
        x = -24;
        y = 54;
        break;
    default:
        x = 0;
        y = 0;
    }
}
