#pragma once

#include "Modules/standard.hpp"
#include "Modules/GUI2D/Widget.hpp"

namespace gui
{

class ButtonWidget : public Widget
{
public:
    ButtonWidget();
    ButtonWidget(const char* text);

    void SetText(const char* text);
    void SetTextColor(const vh::Color& color);
    void SetFont(TTF_Font* font);
    void SetBorder(const vh::Color& color);

    virtual void Draw(int32_t x, int32_t y);

protected:
    virtual SDL_Cursor* GetCursor();

private:
    TTF_Font* mFont;
    std::string mText;
    vh::Color mTextColor;
    vh::Color mBorderColor;
};

}
