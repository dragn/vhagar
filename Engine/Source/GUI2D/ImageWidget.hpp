#pragma once

#include "Widget.hpp"

namespace gui
{

class ImageWidget
{
    ImageWidget(SDL_Surface* surf)
        : mSurf(surf)
    {}

private:
    SDL_Surface* mSurf;
};

}
