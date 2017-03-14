#pragma once

#include "Common.hpp"
#include "Vhagar.hpp"
#include "GUI2D/GUI2D.hpp"
#include "GUI2D/ButtonWidget.hpp"
#include "GUI2D/TextWidget.hpp"
#include "GUI2D/ImageWidget.hpp"
#include "GUI2D/TextFieldWidget.hpp"

#include "Renderer2D/Renderer2D.hpp"

extern const char* ORG_NAME;
extern const char* APP_NAME;

#include <string>
#include <fstream>
#include <random>

namespace Colors
{
    static const vh::Color Orange(0xfe, 0x83, 0x1e);
    static const vh::Color White(0xff, 0xff, 0xff);
    static const vh::Color Blue(0xa0, 0xa0, 0xc0);
    static const vh::Color Transparent(0x00, 0x00, 0x00, 0x00);
}
