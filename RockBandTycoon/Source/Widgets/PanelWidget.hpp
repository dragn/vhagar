#pragma once

#include "GUI2D/Widget.hpp"

class PanelWidget : public gui::Widget
{
public:
    virtual void Draw(int32_t x, int32_t y) override;
};
