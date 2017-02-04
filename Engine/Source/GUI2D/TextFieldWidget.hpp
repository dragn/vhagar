#pragma once

#include "Widget.hpp"
#include <string>

namespace gui
{

class TextFieldWidget : public Widget
{
public:
    TextFieldWidget();

    virtual void OnClick(int32_t x, int32_t y) override;
    virtual void Draw(int32_t x, int32_t y) override;

    virtual void OnFocus() override;
    virtual void OnBlur() override;

private:
    std::string mContent;
};

} // namespace gui
