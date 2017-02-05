#pragma once

#include "Renderer2D/Color.hpp"
#include "Widget.hpp"
#include <list>

namespace gui
{

class View
{
    friend class GUI2D;

public:
    View();
    virtual ~View() {};

    void AddWidget(Widget* widget);

    void SetBackground(const vh::Color& color);
    void SetBackground(const char* imagePath);

private:
    virtual void Render(GUI2D* gui);

    GUI2D* mGUI;

    Widget mRootWidget;
};

}
