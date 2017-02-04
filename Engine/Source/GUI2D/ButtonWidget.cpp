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
}

gui::ButtonWidget::ButtonWidget(const char* text)
    : mHandler(StubHandler)
{
    mText = new TextWidget(text);
    AddChild(mText);
    mText->SetPos(gui::ePos::Center, gui::ePos::Center, gui::eAnchor::CenterCenter);
}

void gui::ButtonWidget::Draw(int32_t x, int32_t y)
{
    int32_t width, height;
    GetSize(width, height);

    vh::Renderer2D* render = vh::App::Get<vh::Renderer2D>();
    CHECK(render);

    vh::Color borderColor(0xb0, 0xb0, 0xb0, 0xff);

    render->DrawRect(x, y, width, height, borderColor);
}

void gui::ButtonWidget::OnClick(int32_t x, int32_t y)
{
    mHandler();
}
