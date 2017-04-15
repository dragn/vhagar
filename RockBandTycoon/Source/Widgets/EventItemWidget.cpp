#include "PCH.hpp"
#include "EventItemWidget.hpp"
#include "StatWidget.hpp"

using namespace vh;
using namespace gui;

static const int32_t WIDTH = 100;
static const int32_t HEIGHT = 150;

EventItemWidget::EventItemWidget(int idx, GameProfile* profile, const EventItem& item)
    : mIdx(idx), mProfile(profile), mItem(item), mFont(nullptr), mCursor(nullptr)
{
    SetSize(WIDTH, HEIGHT);
    SetBackground(Colors::White);

    GUI2D* gui = App::Get<GUI2D>();
    if (gui)
    {
        mFont = gui->GetHdr2Font();
    }

    RepStatWidget* repWdg = new RepStatWidget(item.GetReputation());
    repWdg->SetPos(10, 30);
    AddChild(repWdg);

    PopStatWidget* popWdg = new PopStatWidget(item.GetPopularity());
    popWdg->SetPos(10, 50);
    AddChild(popWdg);
}

void EventItemWidget::Draw(int32_t x, int32_t y)
{
    Renderer2D* render = App::Get<Renderer2D>();
    CHECK(render);

    if (IsFocused()) render->DrawRect(x, y, WIDTH, HEIGHT, Colors::Orange);

    int32_t hdrW, hdrH;
    int32_t w, h; GetSize(w, h);
    render->CalcTextSize(mFont, mItem.GetName().c_str(), hdrW, hdrH);
    render->DrawText(mFont, mItem.GetName().c_str(), Colors::Orange, x + w / 2 - hdrW / 2, y + 5);
}

SDL_Cursor* EventItemWidget::GetCursor()
{
    if (mCursor != nullptr) return mCursor;
    gui::GUI2D* gui = vh::App::Get<gui::GUI2D>();
    CHECK(gui);
    mCursor = gui->GetHandCursor();
    return mCursor;
}
