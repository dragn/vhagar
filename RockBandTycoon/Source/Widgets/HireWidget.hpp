#pragma once

#include "GUI2D/Widget.hpp"
#include "Gameplay/HireItem.hpp"

class HireWidget : public gui::Widget
{
public:
    HireWidget();

    void SetHireItem(const HireItem& item);
    const HireItem& GetHireItem() const;

    virtual void Draw(int32_t x, int32_t y) override;
    virtual void UpdateSize() override {};

private:
    HireItem mHireItem;

    SDL_Surface* mHairImg;
    SDL_Surface* mFaceImg;
    SDL_Surface* mBodyImg;
    SDL_Surface* mHandsImg;
    SDL_Surface* mLegsImg;
};
