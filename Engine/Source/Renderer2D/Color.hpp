#pragma once

namespace vh
{

struct Color
{
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff)
        : r(r), g(g), b(b), a(a)
    {}

    Color(uint8_t gray, uint8_t a = 0xff)
        : r(gray), g(gray), b(gray), a(a)
    {}

    Color() : r(0x00), g(0x00), b(0x00), a(0x00)
    {}

    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0xff;
};

} // namespace vh
