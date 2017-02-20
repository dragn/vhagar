#include "Common.hpp"
#include "ButtonWidget.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"
#include "GUI2D/GUI2D.hpp"

gui::ButtonWidget::ButtonWidget()
    : mFont(nullptr)
    , mTextColor(vh::Color(0xff))
    , mBorderColor(vh::Color(0x00, 0x00))
{
    SetBackground(vh::Color(0x10));
}

gui::ButtonWidget::ButtonWidget(const char* text)
    : mText(text)
    , mFont(NULL)
    , mTextColor(vh::Color(0xff))
    , mBorderColor(vh::Color(0x00, 0x00))
{
    SetBackground(vh::Color(0x10));
}

void gui::ButtonWidget::SetText(const char* text)
{
    mText = text;
    SetDirty();
}

void gui::ButtonWidget::SetTextColor(const vh::Color& color)
{
    mTextColor = color;
    SetDirty();
}

void gui::ButtonWidget::SetFont(TTF_Font* font)
{
    mFont = font;
    SetDirty();
}

void gui::ButtonWidget::SetBorder(const vh::Color& color)
{
    mBorderColor = color;
}

void gui::ButtonWidget::Draw(int32_t x, int32_t y)
{
    vh::Renderer2D* render = vh::App::Get<vh::Renderer2D>();
    CHECK(render);

    TTF_Font* font = mFont;
    if (font == nullptr)
    {
        gui::GUI2D* gui = vh::App::Get<gui::GUI2D>();
        CHECK(gui);
        font = gui->GetFont();
    }

    if (mText.size() && font != nullptr)
    {
        int32_t w, h, textW, textH;
        GetSize(w, h);
        render->CalcTextSize(font, mText.c_str(), textW, textH);

        render->DrawText(font, mText.c_str(), mTextColor, x + w / 2 - textW / 2, y + h / 2 - textH / 2);

        if (mBorderColor.a != 0x00)
        {
            render->DrawRect(x, y, w, h, mBorderColor);
        }
    }
}

SDL_Cursor* gui::ButtonWidget::GetCursor()
{
    if (mCursor != nullptr) return mCursor;
    gui::GUI2D* gui = vh::App::Get<gui::GUI2D>();
    CHECK(gui);
    mCursor = gui->GetHandCursor();
    return mCursor;
}
