#pragma once

#include "Widget.hpp"
#include <list>

namespace gui
{

class View
{
    friend class GUI2D;

public:
    View();

    void AddWidget(Widget* widget);

private:
    virtual void Render(GUI2D* gui);

    GUI2D* mGUI;

    Widget mRootWidget;
};

}
