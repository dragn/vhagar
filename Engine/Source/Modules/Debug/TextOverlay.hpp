#pragma once

#include "Modules/standard.hpp"

namespace vh
{

    class TextOverlay
    {
    public:
        TextOverlay(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

        void Create();
        void Destroy();

        void SetText(const char* text);

    private:
        uint32_t mPosX;
        uint32_t mPosY;
        uint32_t mWidth;
        uint32_t mHeight;

        TTF_Font* mFont;

        std::weak_ptr<Actor> mActor;
    };

} // namespace vh
