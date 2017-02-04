#include "Common.hpp"
#include "GUI2D.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"

namespace gui
{

void GUI2D::TickInit(uint32_t delta)
{
    vh::App::CheckRequired<vh::Renderer2D>();

    TTF_Init();
    CHECK(TTF_WasInit()) << "Error initialising SDL_ttf";

    mFont = TTF_OpenFont(mOptions.fontPath, mOptions.fontSize);
    CHECK(mFont) << "Could not open font: " << mOptions.fontPath;

    vh::Renderer2D* renderer = vh::App::Get<vh::Renderer2D>();
    CHECK(renderer);
    mScale = renderer->GetScale();

    FinishInit();
}

void GUI2D::TickRun(uint32_t delta)
{
    if (mActiveView != nullptr)
    {
        mActiveView->Render(this);
    }

    if (mNextView != nullptr)
    {
        // switch to new view
        if (mActiveView) delete mActiveView;
        mActiveView = mNextView;
        mNextView = nullptr;

        // stretch root widget to all available space
        vh::Renderer2D* renderer = vh::App::Get<vh::Renderer2D>();
        CHECK(renderer);

        mActiveView->mRootWidget.SetSize(renderer->GetWidth(), renderer->GetHeight());
    }
}

void GUI2D::TickClose(uint32_t delta)
{
    if (mFont) TTF_CloseFont(mFont);

    if (TTF_WasInit()) TTF_Quit();

    if (mActiveView) delete mActiveView;
    if (mNextView) delete mNextView;

    FinishClose();
}

void GUI2D::SetView(View* view)
{
    if (mNextView != nullptr) delete mNextView;
    mNextView = view;
}

void GUI2D::HandleEvent(SDL_Event* event)
{
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            int32_t x = event->button.x / mScale;
            int32_t y = event->button.y / mScale;

            if (mActiveView)
            {
                mActiveView->mRootWidget.OnClick(x, y);
            }
        }
    }
}

void GUI2D::CalcTextSize(const char* text, int32_t& outWidth, int32_t& outHeight)
{
    CHECK(mFont);
    CHECK(text);

    SDL_Color any = { 0,0,0,0 };

    SDL_Surface* surf = TTF_RenderText_Solid(mFont, text, any);
    CHECK(surf) << "Render text error";

    outWidth = surf->w;
    outHeight = surf->h;

    SDL_FreeSurface(surf);
}

void GUI2D::DrawText(const char* text, int32_t x, int32_t y)
{
    vh::Renderer2D* renderer = vh::App::Get<vh::Renderer2D>();
    CHECK(renderer);

    renderer->DrawText(mFont, text, x, y);
}

}
