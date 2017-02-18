#pragma once

#include "Utils/Delegate.hpp"
#include "Widget.hpp"
#include "TextWidget.hpp"

namespace gui
{

class ButtonWidget : public Widget
{
public:
    ButtonWidget();
    ButtonWidget(const char* text);

    virtual void OnClick(int32_t x, int32_t y) override;

    template<typename M>
    void SetOnClickHandler(M handler)
    {
        mOnClickDelegate.Set(handler);
    }

    template<typename T, typename M>
    void SetOnClickHandler(T t, M handler)
    {
        mOnClickDelegate.Set(t, handler);
    }

    void SetText(const char* text);
    void SetTextColor(const vh::Color& color);
    void SetFont(TTF_Font* font);

    virtual void Draw(int32_t x, int32_t y);

protected:
    virtual SDL_Cursor* GetCursor();

private:
    vh::Delegate mOnClickDelegate;

    TTF_Font* mFont;
    std::string mText;
    vh::Color mTextColor;
};

}
