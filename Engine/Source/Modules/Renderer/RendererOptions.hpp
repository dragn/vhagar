#pragma once

namespace vh
{

    struct RendererOptions
    {
        const char *resourceRoot = ".";
        int screenWidth = 1024;
        int screenHeight = 768;
        bool borderless = true;

        enum
        {
            AA_OFF = 0, // Antialiasing turned off
            AA_2X,      // MSAAx2
            AA_4X       // MSAAx4
        } antialias = AA_OFF;

        enum
        {
            MON_FIRST = 0,
            MON_SECOND
        } monitor = MON_FIRST;
    };

} // namespace vh
