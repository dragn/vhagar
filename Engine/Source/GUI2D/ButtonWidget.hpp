#pragma once

#include "Widget.hpp"

namespace gui
{

class ButtonWidget : public Widget
{
public:
    virtual void Draw(int32_t x, int32_t y) override;
};

}
