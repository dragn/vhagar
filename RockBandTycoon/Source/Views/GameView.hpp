#pragma once

#include "GUI2D/View.hpp"
#include "Gameplay/GameProfile.hpp"

class GameView : public gui::View
{
public:
    GameView(int slot);
    virtual ~GameView();

private:
    GameProfile* mProfile;
};
