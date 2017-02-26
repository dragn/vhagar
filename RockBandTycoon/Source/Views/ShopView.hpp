#pragma once

#include "GUI2D/View.hpp"
#include "Gameplay/GameProfile.hpp"
#include "Components/Resources.hpp"
#include "GUI2D/ButtonWidget.hpp"
#include "Widgets/BandMemberWidget.hpp"

class ShopView : public gui::View
{
public:
    ShopView(GameProfile* profile);

    void SetType(eBandSlot::Type type);

private:
    GameProfile* mProfile;

    eBandSlot::Type mType;

    gui::ButtonWidget* mBtn[eBandSlot::MAX];

    gui::Widget* mList;
    BandMemberWidget* mMemberWidget;

    void DrawItems();
};
