#include "Common.hpp"
#include "TextWidget.hpp"
#include "GUI2D/GUI2D.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"

using vh::App;
using vh::Renderer2D;

gui::TextWidget::TextWidget()
{
}

void gui::TextWidget::SetText(const char* text)
{
    mText = text;
    SetDirty();
}

void gui::TextWidget::Draw(int32_t x, int32_t y)
{
    if (mText.empty()) return;

    gui::GUI2D* gui = vh::App::Get<gui::GUI2D>();
    CHECK(gui);

    gui->DrawText(mText.c_str(), x, y);
}

gui::TextWidget::TextWidget(const char* text)
{
    SetText(text);
}

void gui::TextWidget::UpdateSize()
{
    if (mText.empty())
    {
        SetSize(0, 0);
        return;
    }

    gui::GUI2D* gui = vh::App::Get<gui::GUI2D>();
    CHECK(gui);

    int32_t width, height;
    gui->CalcTextSize(mText.c_str(), width, height);

    SetSize(width, height);
}
