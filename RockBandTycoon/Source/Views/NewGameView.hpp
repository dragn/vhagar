#pragma once

#include "GUI2D/View.hpp"

class NewGameView : public gui::View
{
public:
    NewGameView(int slot);

    void OnStart();

private:
    int mSlot;
};
