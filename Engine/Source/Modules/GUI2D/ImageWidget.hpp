#pragma once

#include "Modules/GUI2D/Widget.hpp"
#include "Modules/standard.hpp"

namespace gui
{

class ImageWidget : public Widget
{
public:
    ImageWidget(const char* filename);
    ImageWidget(SDL_Surface* surf);
    ImageWidget();

    virtual ~ImageWidget();

    virtual void Draw(int32_t x, int32_t y) override;
    virtual void UpdateSize() override;

private:
    SDL_Surface* mSurf;
};

}
