#pragma once

#include "GUI2D/View.hpp"

class MenuView : public gui::View
{
public:
    MenuView();

private:
    void AddSlotButton(int slot);
    bool GetProfile(std::string& outName, int slot);
};
