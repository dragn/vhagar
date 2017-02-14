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

private:
    GameProfile* mProfile;

    gui::TextWidget* mBandNameTxt;

    void CreateToolbar();
};
