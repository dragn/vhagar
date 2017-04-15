#pragma once

#include "GUI2D/View.hpp"
#include "Gameplay/GameProfile.hpp"
#include "GUI2D/TextWidget.hpp"

class GameView : public gui::View
{
public:
    GameView(int slot);
    virtual ~GameView();

    GameProfile* GetProfile() const { return mProfile; }

    void HandleMemberChange(const BandMember& member);

    virtual void Render() override;

private:
    GameProfile* mProfile;

    gui::TextWidget* mBandNameTxt;

    void CreateToolbar();

    gui::Widget* mMemberWidgets[eBandSlot::MAX];
};
