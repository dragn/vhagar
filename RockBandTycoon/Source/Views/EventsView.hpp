#pragma once

#include "GUI2D/View.hpp"
#include "Gameplay/GameProfile.hpp"

class EventsView : public gui::View
{
public:
    EventsView(GameProfile* profile);

private:
    GameProfile* mProfile;
};
