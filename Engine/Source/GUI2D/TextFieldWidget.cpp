#include "Vhagar_PCH.hpp"

#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"
#include "TextFieldWidget.hpp"
#include "GUI2D.hpp"

gui::TextFieldWidget::TextFieldWidget()
    : mMaxSize(20)
    , mBlink(false)
{
    SetBackground(vh::Color(0x00));
}

gui::TextFieldWidget::~TextFieldWidget()
{
    SDL_RemoveTimer(mTimer);
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

    if (IsFocused() && mBlink) mContent.append(1, '|');
    int32_t textW, textH;
    renderer->CalcTextSize(gui->GetFont(), mContent.c_str(), textW, textH);
    renderer->DrawText(gui->GetFont(), mContent.c_str(), vh::Color(0xff), x + 4, y + height / 2 - textH / 2, width - 8);
    if (IsFocused() && mBlink && mContent.back() == '|') mContent.pop_back();
}

SDL_Cursor* gui::TextFieldWidget::GetCursor()
{
    if (mCursor != nullptr) return mCursor;
    GUI2D* gui = vh::App::Get<GUI2D>();
    CHECK(gui);
    mCursor = gui->GetBeamCursor();
    return mCursor;
}

Uint32 gui::TextFieldWidget::TimerCallback(Uint32 interval, void* param)
{
    CHECK(param);
    gui::TextFieldWidget* widget = reinterpret_cast<gui::TextFieldWidget*>(param);
    widget->mBlink = !widget->mBlink;
    widget->SetDirty();
    return interval;
}

void gui::TextFieldWidget::OnFocusInternal()
{
    Widget::OnFocusInternal();

    mBlink = true;
    mTimer = SDL_AddTimer(1 << 9, &TimerCallback, this);
}

void gui::TextFieldWidget::OnBlurInternal()
{
    Widget::OnBlurInternal();

    SDL_RemoveTimer(mTimer);
}

void gui::TextFieldWidget::HandleEvent(SDL_Event* event)
{
    IF_KEYDOWN_SYM(event, SDLK_BACKSPACE)
    {
        if (mContent.size())
        {
            mContent.pop_back();
            OnChange(mContent);
            SetDirty();
        }
    }
    if (event->type == SDL_TEXTINPUT)
    {
        if (mContent.size() < mMaxSize)
        {
            mContent.append(event->text.text);
            OnChange(mContent);
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
    OnChange(mContent);
    SetDirty();
}

const char* gui::TextFieldWidget::GetValue() const
{
    return mContent.c_str();
}
