#pragma once

#include "SDL.h"
#include "GUI2D/Widget.hpp"
#include "Utils/Property.hpp"

class StatWidget : public gui::Widget
{
    VH_PROPERTY_RW(float, Value);
    VH_PROPERTY_RO(float, Multiplier);

public:
    StatWidget(const char* img, float multiplayer, float val);
    ~StatWidget();

    virtual void Draw(int32_t x, int32_t y) override;

private:
    void OnValueChange(float val);
    SDL_Surface* mStatImg;
    SDL_Surface* mUpImg;
    SDL_Surface* mDownImg;
};

class RepStatWidget : public StatWidget
{
public:
    RepStatWidget(float val) : StatWidget("Assets/Images/rep.png", 10.0f, val) {}
};

class PopStatWidget : public StatWidget
{
public:
    PopStatWidget(float val) : StatWidget("Assets/Images/pop.png", 10.0f, val) {}
};
