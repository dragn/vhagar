#pragma once

struct Renderer2DOptions
{
    uint32_t screenWidth = 1024;
    uint32_t screenHeight = 768;
    uint8_t scale = 1;

    enum
    {
        FS_Window = 0,
        FS_Fullscreen,
        FS_Borderless
    } fullscreen = FS_Window;

    enum
    {
        MON_First = 0,
        MON_Second
    } monitor = MON_First;
};
