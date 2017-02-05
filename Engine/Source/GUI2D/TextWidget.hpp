#pragma once

#include "Widget.hpp"

namespace gui
{

class TextWidget : public Widget
{
public:
    TextWidget();
    TextWidget(const char* text);

    void SetText(const char* text);

    virtual void Draw(int32_t x, int32_t y) override;

protected:
    virtual void UpdateSize() override;

private:
    std::string mText;
};

}
