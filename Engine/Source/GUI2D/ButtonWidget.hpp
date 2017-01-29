#pragma once

#include "Widget.hpp"
#include "TextWidget.hpp"

namespace gui
{

typedef void (*OnClickHandler)();

class ButtonWidget : public Widget
{
public:
    ButtonWidget();
    ButtonWidget(const char* text);

    virtual void Draw(int32_t x, int32_t y) override;
    virtual void OnClick(int32_t x, int32_t y) override;

    void SetOnClickHandler(OnClickHandler handler)
    {
        mHandler = handler;
    }

private:
    OnClickHandler mHandler;
    TextWidget* mText;
};

}
