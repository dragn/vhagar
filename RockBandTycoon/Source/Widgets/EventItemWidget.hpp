#pragma once

#include "SDL_ttf.h"
#include "GUI2D/Widget.hpp"
#include "Gameplay/EventItem.hpp"
#include "Gameplay/GameProfile.hpp"

class EventItemWidget : public gui::Widget
{
public:
    EventItemWidget(int idx, GameProfile* profile, const EventItem& item);

    virtual void Draw(int32_t x, int32_t y) override;

protected:
    virtual bool IsFocusable() const override { return true; }
    virtual SDL_Cursor* GetCursor() override;

private:
    int mIdx;
    GameProfile* mProfile;
    EventItem mItem;
    TTF_Font* mFont;
    SDL_Cursor* mCursor;
};
