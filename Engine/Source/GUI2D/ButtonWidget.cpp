#include "Common.hpp"
#include "ButtonWidget.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"

void StubHandler()
{
    LOG(INFO) << "Button click handler not set!";
}

gui::ButtonWidget::ButtonWidget()
    : mHandler(StubHandler)
{
    mText = new TextWidget();
    AddChild(mText);
    mText->SetPos(gui::ePos::Center, gui::ePos::Center, gui::eAnchor::CenterCenter);

    SetBackground(vh::Color(0x10));
}

gui::ButtonWidget::ButtonWidget(const char* text)
    : mHandler(StubHandler)
{
    mText = new TextWidget(text);
    AddChild(mText);
    mText->SetPos(gui::ePos::Center, gui::ePos::Center, gui::eAnchor::CenterCenter);

    SetBackground(vh::Color(0x10));
}

void gui::ButtonWidget::OnClick(int32_t x, int32_t y)
{
    mHandler();
}
