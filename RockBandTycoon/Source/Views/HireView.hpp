#pragma once

#include "GUI2D/View.hpp"
#include "Gameplay/GameProfile.hpp"
#include "Gameplay/HireItem.hpp"

class HireView : public gui::View
{
public:
    HireView(eBandSlot::Type type, GameProfile* profile);

    void OnHire(int idx);

    const GameProfile* GetProfile() const
    {
        return mProfile;
    }

private:
    eBandSlot::Type mType;
    GameProfile* mProfile;

    const std::vector<HireItem>& GetHires();
};
