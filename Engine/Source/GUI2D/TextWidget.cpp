#include "Common.hpp"
#include "TextWidget.hpp"
#include "GUI2D/GUI2D.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"

using vh::App;
using vh::Renderer2D;

gui::TextWidget::TextWidget()
    : mText(nullptr)
{
}

void gui::TextWidget::SetText(const char* text)
{
    mText = text;
    SetDirty();
}

void gui::TextWidget::Draw(int32_t x, int32_t y)
{
    if (mText == nullptr) return;

    gui::GUI2D* gui = vh::App::Get<gui::GUI2D>();
    CHECK(gui);

    gui->DrawText(mText, x, y);
}

gui::TextWidget::TextWidget(const char* text)
{
    SetText(text);
}

void gui::TextWidget::UpdateSize()
{
    if (mText == nullptr)
    {
        SetSize(0, 0);
        return;
    }

    gui::GUI2D* gui = vh::App::Get<gui::GUI2D>();
    CHECK(gui);

    int32_t width, height;
    gui->CalcTextSize(mText, width, height);

    SetSize(width, height);
}
