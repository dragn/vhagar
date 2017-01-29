#include "Common.hpp"
#include "ButtonWidget.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"

void gui::ButtonWidget::Draw(int32_t x, int32_t y)
{
    int32_t width, height;
    GetSize(width, height);

    vh::Renderer2D* render = vh::App::Get<vh::Renderer2D>();
    CHECK(render);

    vh::Color borderColor(0xb0, 0xb0, 0xb0, 0xff);

    render->DrawRect(x, y, width, height, borderColor);
}
