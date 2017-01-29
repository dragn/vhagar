#include "Common.hpp"
#include "GUI2D.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"

namespace gui
{

void GUI2D::TickInit(uint32_t delta)
{
    vh::App::CheckRequired<vh::Renderer2D>();
    FinishInit();
}

void GUI2D::TickRun(uint32_t delta)
{
    if (mActiveView != nullptr) mActiveView->Render(this);
}

void GUI2D::TickClose(uint32_t delta)
{
    FinishClose();
}

void GUI2D::SetView(View* view)
{
    if (mActiveView)
    {
        delete mActiveView;
    }

    vh::Renderer2D* renderer = vh::App::Get<vh::Renderer2D>();
    CHECK(renderer);

    mActiveView = view;

    // stretch root widget to all available space
    mActiveView->mRootWidget.SetSize(renderer->GetWidth(), renderer->GetHeight());
}

}
