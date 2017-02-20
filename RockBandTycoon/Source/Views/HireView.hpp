#pragma once

#include "GUI2D/View.hpp"
#include "Gameplay/GameProfile.hpp"
#include "Gameplay/HireItem.hpp"

class HireView : public gui::View
{
public:
    HireView(eBandSlot::Type type, GameProfile* profile);

    void OnHire(HireItem item);

private:
    eBandSlot::Type mType;
    GameProfile* mProfile;
};
