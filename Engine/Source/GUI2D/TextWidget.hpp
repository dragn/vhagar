#pragma once

#include "Widget.hpp"
#include "Renderer2D/Color.hpp"
#include "Utils/Delegate.hpp"

namespace gui
{

class TextWidget : public Widget
{
public:
    TextWidget(const char* text = nullptr);
    virtual ~TextWidget();

    void SetText(const char* text);
    void SetText(const std::string& text);

    void SetFont(TTF_Font* font);
    void SetColor(vh::Color color);

    virtual void Draw(int32_t x, int32_t y) override;

    void BindTo(vh::MultiDelegate<const std::string&>& del);
    void Unbind();

protected:
    virtual void UpdateSize() override;

private:
    TTF_Font* mFont;
    std::string mText;
    vh::Color mColor;

    vh::MultiDelegate<const std::string&>* mBoundDelegate;
};

}
