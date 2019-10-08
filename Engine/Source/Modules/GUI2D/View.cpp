#include "Vhagar_PCH.hpp"

#include "Utils/Logging.hpp"
#include "Utils/Defs.hpp"
#include "View.hpp"

namespace gui
{

void gui::View::AddWidget(Widget* widget)
{
    mRootWidget->AddChild(widget);
}

void gui::View::Render()
{
    if (mRootWidget != nullptr) mRootWidget->Draw(nullptr);
}

View::View(const char* name /*= "NonameView"*/)
    : mRootWidget(nullptr)
    , mName(name)
{
    mRootWidget = new Widget();
}

View::~View()
{
    Destroy();
}

void View::RemoveWidget(Widget* widget)
{
    mRootWidget->RemoveChild(widget);
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
    vh::SafeDelete(mRootWidget);
    OnDestroy();
}

}
