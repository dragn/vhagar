#include "PCH.hpp"
#include "StatWidget.hpp"
#include "Components/Resources.hpp"
#include "GUI2D/ImageWidget.hpp"

using namespace vh;
using namespace gui;

StatWidget::StatWidget(const char* img, float multiplayer, float val)
    : mMultiplier(multiplayer)
    , mValue(val)
    , mStatImg(nullptr)
    , mUpImg(nullptr)
    , mDownImg(nullptr)
{
    mStatImg = IMG_Load(img);
    mUpImg = IMG_Load("Assets/Images/up.png");
    mDownImg = IMG_Load("Assets/Images/down.png");

    Value_OnChange.Add(this, &StatWidget::OnValueChange);
}

void StatWidget::OnValueChange(float val)
{
    SetDirty();
}

StatWidget::~StatWidget()
{
    SDL_FreeSurface(mStatImg);
    SDL_FreeSurface(mUpImg);
    SDL_FreeSurface(mDownImg);
}

void StatWidget::Draw(int32_t x, int32_t y)
{
    Renderer2D* render = App::Get<Renderer2D>();

    if (mStatImg) render->DrawImage(mStatImg, x, y);

    if (mValue > 0.0f)
    {
        for (int i = 0; i < 4 && mValue > i * mMultiplier; ++i)
        {
            render->DrawImage(mUpImg, x + mStatImg->w + i * mUpImg->w, y);
        }
    }
    else if (mValue < 0.0f)
    {
        for (int i = 0; i < 4 && mValue < - i * mMultiplier; ++i)
        {
            render->DrawImage(mDownImg, x + mStatImg->w + i * mDownImg->w, y);
        }
    }
}
