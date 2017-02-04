#include "Common.hpp"

#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"
#include "TextFieldWidget.hpp"
#include "GUI2D.hpp"

gui::TextFieldWidget::TextFieldWidget()
{
    SetBackground(vh::Color(0x00));
}

void gui::TextFieldWidget::OnClick(int32_t x, int32_t y)
{
    GUI2D* gui = vh::App::Get<GUI2D>();
    CHECK(gui);

    gui->SetFocus(this);
}

void gui::TextFieldWidget::Draw(int32_t x, int32_t y)
{
    vh::Renderer2D* renderer = vh::App::Get<vh::Renderer2D>();
    CHECK(renderer);

    GUI2D* gui = vh::App::Get<GUI2D>();
    CHECK(gui);

    int32_t width, height;
    GetSize(width, height);

    renderer->DrawRect(x, y, width, height, vh::Color(0xff));

    int32_t textW, textH;
    gui->CalcTextSize(mContent.c_str(), textW, textH);
    renderer->DrawText(gui->GetFont(), mContent.c_str(), x + 2, y + height / 2 - textH / 2);
}

void gui::TextFieldWidget::OnFocus()
{
    mContent.append(1, '|');
    SetDirty();
}

void gui::TextFieldWidget::OnBlur()
{
    if (!mContent.empty() && mContent.back() == '|') mContent.pop_back();
    SetDirty();
}
