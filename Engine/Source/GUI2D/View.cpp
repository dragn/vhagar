#include "Common.hpp"

#include "View.hpp"

namespace gui
{

void gui::View::AddWidget(Widget* widget)
{
    mRootWidget->AddChild(widget);
}

void gui::View::Render(GUI2D* gui)
{
    mGUI = gui;
    if (mRootWidget != nullptr) mRootWidget->Draw(nullptr);
}

View::View(const char* name /*= "NonameView"*/)
    : mName(name)
    , mRootWidget(nullptr)
{
    mRootWidget = new Widget();
}

View::~View()
{
    Destroy();
}

void View::SetBackground(const vh::Color& color)
{
    CHECK(mRootWidget);

    mRootWidget->SetBackground(color);
}

void View::SetBackground(const char* imagePath)
{
    CHECK(mRootWidget);

    mRootWidget->SetBackground(imagePath);
}

void View::Destroy()
{
    if (mRootWidget != nullptr)
    {
        delete mRootWidget;
        mRootWidget = nullptr;
    }
}

}
