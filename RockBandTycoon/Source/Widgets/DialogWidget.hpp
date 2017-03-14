#pragma once

#include "SDL_ttf.h"
#include "GUI2D/Widget.hpp"

class DialogWidget : public gui::Widget
{
public:
    DialogWidget(const std::string& text);

    virtual void Draw(int32_t x, int32_t y) override;

private:
    const std::string mText;
    TTF_Font* mFont;
};
