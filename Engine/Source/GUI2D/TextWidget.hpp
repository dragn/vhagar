#pragma once

#include "Widget.hpp"
#include "Renderer2D/Color.hpp"

namespace gui
{

class TextWidget : public Widget
{
public:
    TextWidget(const char* text = nullptr);

    void SetText(const char* text);

    void SetFont(TTF_Font* font);
    void SetColor(vh::Color color);

    virtual void Draw(int32_t x, int32_t y) override;

protected:
    virtual void UpdateSize() override;

private:
    TTF_Font* mFont;
    std::string mText;
    vh::Color mColor;
};

}
