#include "Common.hpp"
#include "TextWidget.hpp"
#include "GUI2D/GUI2D.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"

using vh::App;
using vh::Renderer2D;
using gui::GUI2D;

gui::TextWidget::TextWidget(const char* text /* = nullptr */)
    : mColor(0xff)
    , mFont(nullptr)
    , mBoundDelegate(nullptr)
{
    if (text != nullptr) SetText(text);
}

gui::TextWidget::~TextWidget()
{
    Unbind();
}

void gui::TextWidget::SetText(const char* text)
{
    mText = text;
    SetDirty();
}

void gui::TextWidget::SetText(const std::string& text)
{
    SetText(text.c_str());
}

void gui::TextWidget::SetFont(TTF_Font* font)
{
    CHECK(font);
    mFont = font;
    SetDirty();
}

void gui::TextWidget::SetColor(vh::Color color)
{
    mColor = color;
}

void gui::TextWidget::Draw(int32_t x, int32_t y)
{
    if (mText.empty()) return;

    vh::Renderer2D* renderer = vh::App::Get<vh::Renderer2D>();
    CHECK(renderer);

    renderer->DrawText(mFont, mText.c_str(), mColor, x, y);
}

void gui::TextWidget::BindTo(vh::MultiDelegate<const std::string&>& del)
{
    if (mBoundDelegate != nullptr)
    {
        mBoundDelegate->Remove<TextWidget*, void (TextWidget::*)(const std::string&)>(this, &TextWidget::SetText);
    }
    del.Add<TextWidget*, void (TextWidget::*)(const std::string&)>(this, &TextWidget::SetText);
    mBoundDelegate = &del;
}

void gui::TextWidget::Unbind()
{
    if (mBoundDelegate != nullptr)
    {
        mBoundDelegate->Remove<TextWidget*, void (TextWidget::*)(const std::string&)>(this, &TextWidget::SetText);
        mBoundDelegate = nullptr;
    }
}

void gui::TextWidget::UpdateSize()
{
    if (mFont == nullptr)
    {
        GUI2D* gui = App::Get<GUI2D>();
        CHECK(gui);
        mFont = gui->GetFont();
    }

    if (mText.empty())
    {
        SetSize(0, 0);
        return;
    }

    vh::Renderer2D* renderer = vh::App::Get<vh::Renderer2D>();
    CHECK(renderer);

    int32_t width, height;
    renderer->CalcTextSize(mFont, mText.c_str(), width, height);

    SetSize(width, height);
}
