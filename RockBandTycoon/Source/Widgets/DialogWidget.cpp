#include "PCH.hpp"
#include "DialogWidget.hpp"

using namespace vh;
using namespace gui;

void DialogWidget::Draw(int32_t x, int32_t y)
{
    Renderer2D* render = App::Get<Renderer2D>();

    int32_t w, h; GetSize(w, h);

    if (mText.size())
    {
        render->DrawText(mFont, mText.c_str(), Colors::Orange, x + 4, y + 4, w - 8, true);
    }
}

DialogWidget::DialogWidget(const std::string& text)
    : mText(text)
{
    GUI2D* gui = App::Get<GUI2D>();
    CHECK(gui);

    mFont = gui->GetFont();
}
