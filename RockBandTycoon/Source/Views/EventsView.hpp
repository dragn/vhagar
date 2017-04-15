#pragma once

#include "GUI2D/View.hpp"
#include "Gameplay/GameProfile.hpp"
#include "Widgets/EventItemWidget.hpp"

class EventsView : public gui::View
{
public:
    EventsView(GameProfile* profile);

    void GoOn();

private:
    GameProfile* mProfile;
    std::vector<EventItemWidget*> mWidgets;
};
