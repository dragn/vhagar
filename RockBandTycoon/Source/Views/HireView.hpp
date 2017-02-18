#pragma once

#include "GUI2D/View.hpp"
#include "Gameplay/GameProfile.hpp"

class HireView : public gui::View
{
public:
    HireView(eBandSlot::Type type, GameProfile* profile);

private:
    eBandSlot::Type mType;
    GameProfile* mProfile;
};
