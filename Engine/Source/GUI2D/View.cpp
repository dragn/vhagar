#include "Common.hpp"

#include "View.hpp"

namespace gui
{

void gui::View::AddWidget(Widget* widget)
{
    mRootWidget.AddChild(widget);
}

void gui::View::Render(GUI2D* gui)
{
    mGUI = gui;
    mRootWidget.Draw(nullptr);
}

View::View(const char* name /*= "NonameView"*/)
    : mName(name)
{
}

void View::SetBackground(const vh::Color& color)
{
    mRootWidget.SetBackground(color);
}

void View::SetBackground(const char* imagePath)
{
    mRootWidget.SetBackground(imagePath);
}

}
