#include "PCH.hpp"
#include "EventsView.hpp"
#include "Widgets/EventItemWidget.hpp"

using namespace vh;
using namespace gui;

EventsView::EventsView(GameProfile* profile)
    : mProfile(profile)
{
    SetBackground("Assets/Images/menu_background.png");

    GUI2D* gui = App::Get<GUI2D>();

    ButtonWidget* backBtn = new ButtonWidget("< BACK");
    backBtn->SetSize(73, 28);
    backBtn->SetPos(ePos::Center, 222, eAnchor::TopCenter);
    backBtn->SetBackground(Color(0x00, 0x00));
    backBtn->SetBackground("Assets/Images/button.png");
    backBtn->SetTextColor(Color(0x00));
    backBtn->SetFont(gui->GetHdr2Font());
    backBtn->OnClick.Add([] ()
    {
        GUI2D* gui = App::Get<GUI2D>();
        gui->Back();
    });
    AddWidget(backBtn);

    const std::vector<EventItem>& events = profile->GetEvents();
    int idx = 0;

    for (int idx = 0; idx < events.size() && idx < 4; ++idx)
    {
        EventItemWidget* eventWdg = new EventItemWidget(idx, mProfile, events[idx]);
        eventWdg->SetPos(25 + 110 * idx, 30);
        AddWidget(eventWdg);
    }
}
