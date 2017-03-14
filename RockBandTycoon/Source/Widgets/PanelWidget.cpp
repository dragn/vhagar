#include "PCH.hpp"
#include "PanelWidget.hpp"

using namespace vh;
using namespace gui;

void PanelWidget::Draw(int32_t x, int32_t y)
{
    Renderer2D* render = App::Get<Renderer2D>();
    CHECK(render);

    int32_t w, h; GetSize(w, h);

    render->FillRect(x, y, w, h, Colors::White);
    render->DrawRect(x, y, w, h, Colors::Orange);
    render->DrawRect(x-1, y-1, w+2, h+2, Color(0x00, 0x33));
    render->DrawRect(x-2, y-2, w+4, h+4, Color(0x00, 0x22));
    render->DrawRect(x-3, y-3, w+6, h+6, Color(0x00, 0x11));
}
