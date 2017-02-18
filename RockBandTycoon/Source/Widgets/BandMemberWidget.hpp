#pragma once

#include "Gameplay/BandMember.hpp"
#include "GUI2D/Widget.hpp"

class BandMemberWidget : public gui::Widget
{
public:
    BandMemberWidget()
        : Widget()
        , mHairImg(nullptr)
        , mFaceImg(nullptr)
        , mBodyImg(nullptr)
        , mHandsImg(nullptr)
        , mLegsImg(nullptr)
        , mItemImg(nullptr)
    {
    }

    void SetBandMember(const BandMember& member);
    BandMember GetBandMember() const;

    virtual void Draw(int32_t x, int32_t y) override;

    virtual void UpdateSize() override;

    template<typename T>
    void Bind(vh::MultiDelegate<T>& del)
    {
        del.Add<BandMemberWidget*, void (BandMemberWidget::*)(const T&)>(this, &BandMemberWidget::SetBandMember);
        OnDestroy.Add([&] (Widget* widget)
        {
            del.Remove<BandMemberWidget*, void (BandMemberWidget::*)(const T&)>(reinterpret_cast<BandMemberWidget*>(widget), &BandMemberWidget::SetBandMember);
        });
    }

private:
    BandMember mBandMember;

    SDL_Surface* mHairImg;
    SDL_Surface* mFaceImg;
    SDL_Surface* mBodyImg;
    SDL_Surface* mHandsImg;
    SDL_Surface* mLegsImg;
    SDL_Surface* mItemImg;

    void GetOffset(int32_t& x, int32_t& y);
};
