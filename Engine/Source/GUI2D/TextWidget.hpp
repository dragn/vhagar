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

    template<typename T>
    void SetText(const T& val)
    {
        SetText(std::to_string(val));
    }

    void SetFont(TTF_Font* font);
    void SetColor(vh::Color color);

    virtual void Draw(int32_t x, int32_t y) override;

    void SetDirty() override;

    template<typename T>
    void Bind(vh::MultiDelegate<T>& del)
    {
        del.Add<TextWidget*, void (TextWidget::*)(const T&)>(this, &TextWidget::SetText);
        OnDestroy.Add([&] (Widget* widget)
        {
            del.Remove<TextWidget*, void (TextWidget::*)(const T&)>(reinterpret_cast<TextWidget*>(widget), &TextWidget::SetText);
        });
    }

protected:
    virtual void UpdateSize() override;

private:
    TTF_Font* mFont;
    std::string mText;
    vh::Color mColor;
};

template<> void TextWidget::SetText(const float& val);

}
