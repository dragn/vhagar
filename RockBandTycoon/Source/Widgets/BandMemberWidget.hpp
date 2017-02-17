#pragma once

#include "Gameplay/BandMember.hpp"
#include "GUI2D/Widget.hpp"

class BandMemberWidget : public gui::Widget
{
public:
    BandMemberWidget() : Widget() {}

    void SetBandMember(const BandMember& member);
    BandMember GetBandMember() const;

    virtual void Draw(int32_t x, int32_t y) override;

    virtual void UpdateSize() override;

private:
    BandMember mBandMember;

    SDL_Surface* mHairImg;
    SDL_Surface* mFaceImg;
    SDL_Surface* mBodyImg;
    SDL_Surface* mHandsImg;
    SDL_Surface* mLegsImg;
};
