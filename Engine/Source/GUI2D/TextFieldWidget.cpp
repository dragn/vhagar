#include "Common.hpp"

#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"
#include "TextFieldWidget.hpp"
#include "GUI2D.hpp"

gui::TextFieldWidget::TextFieldWidget()
    : mFocus(false)
    , mMaxSize(20)
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

    if (mFocus) mContent.append(1, '|');
    int32_t textW, textH;
    renderer->CalcTextSize(gui->GetFont(), mContent.c_str(), textW, textH);
    renderer->DrawText(gui->GetFont(), mContent.c_str(), vh::Color(0xff), x + 4, y + height / 2 - textH / 2, width - 8);
    if (mFocus && mContent.back() == '|') mContent.pop_back();
}

void gui::TextFieldWidget::OnFocus()
{
    mFocus = true;
    SetDirty();
}

void gui::TextFieldWidget::OnBlur()
{
    mFocus = false;
    SetDirty();
}

void gui::TextFieldWidget::HandleEvent(SDL_Event* event)
{
    IF_KEYDOWN_SYM(event, SDLK_BACKSPACE)
    {
        if (mContent.size()) mContent.pop_back();
        SetDirty();
    }
    if (event->type == SDL_TEXTINPUT)
    {
        if (mContent.size() < mMaxSize)
        {
            mContent.append(event->text.text);
            SetDirty();
        }
    }
}

void gui::TextFieldWidget::SetMaxSize(uint32_t size)
{
    mMaxSize = size;
}

uint32_t gui::TextFieldWidget::GetMaxSize() const
{
    return mMaxSize;
}

void gui::TextFieldWidget::SetValue(const char* value)
{
    mContent = value;
    SetDirty();
}

const char* gui::TextFieldWidget::GetValue() const
{
    return mContent.c_str();
}
