#include "Common.hpp"
#include "ButtonWidget.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"

gui::ButtonWidget::ButtonWidget()
{
    mText = new TextWidget();
    AddChild(mText);
    mText->SetPos(gui::ePos::Center, gui::ePos::Center, gui::eAnchor::CenterCenter);

    SetBackground(vh::Color(0x10));
}

gui::ButtonWidget::ButtonWidget(const char* text)
{
    mText = new TextWidget(text);
    AddChild(mText);
    mText->SetPos(gui::ePos::Center, gui::ePos::Center, gui::eAnchor::CenterCenter);

    SetBackground(vh::Color(0x10));
}

void gui::ButtonWidget::OnClick(int32_t x, int32_t y)
{
    mOnClickDelegate();
}
