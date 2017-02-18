#pragma once

#include "GUI2D/Widget.hpp"
#include "BandMemberWidget.hpp"

class BandMemberAdWidget : public BandMemberWidget
{
public:
    BandMemberAdWidget() {}

    virtual void Draw(int32_t x, int32_t y) override;
    virtual void UpdateSize() override;
};
