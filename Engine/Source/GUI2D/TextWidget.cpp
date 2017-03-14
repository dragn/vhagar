#include "Common.hpp"
#include "TextWidget.hpp"
#include "GUI2D/GUI2D.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"

using vh::App;
using vh::Renderer2D;
using gui::GUI2D;

gui::TextWidget::TextWidget(const char* text /* = nullptr */)
    : mFont(nullptr)
    , mColor(0xff)
    , mWrap(0)
{
    if (text != nullptr) SetText(text);
}

gui::TextWidget::~TextWidget()
{
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

    if (mWrap > 0)
    {
        renderer->DrawText(mFont, mText.c_str(), mColor, x, y, mWrap, true);
    }
    else
    {
        renderer->DrawText(mFont, mText.c_str(), mColor, x, y);
    }
}

void gui::TextWidget::SetDirty()
{
    Widget::SetDirty();

    // for a text widget it makes sense to also redraw parent,
    // as it is often translucent
    if (mParent) mParent->SetDirty();
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
    renderer->CalcTextSize(mFont, mText.c_str(), width, height, mWrap);

    SetSize(width, height);
}

template<> void gui::TextWidget::SetText(const float& val)
{
    char buf[16];
    snprintf(buf, 16, "%.2g", val);
    SetText((const char*) buf);
}
