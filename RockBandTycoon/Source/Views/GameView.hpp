#pragma once

#include "GUI2D/View.hpp"
#include "Gameplay/GameProfile.hpp"
#include "GUI2D/TextWidget.hpp"

class GameView : public gui::View
{
public:
    GameView(int slot);
    virtual ~GameView();

private:
    GameProfile* mProfile;

    gui::TextWidget* mBandNameTxt;

    void BandName_OnChange(const std::string& name);
};
