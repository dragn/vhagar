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

    mHdrFont = TTF_OpenFont(mOptions.hdrFontPath, mOptions.hdrFontSize);
    CHECK(mHdrFont) << "Could not open font: " << mOptions.hdrFontPath;

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
        /* clear focus */
        SetFocus(nullptr);

        // switch to new view
        if (mActiveView) delete mActiveView;
        mActiveView = mNextView;
        mNextView = nullptr;

        // stretch root widget to all available space
        vh::Renderer2D* renderer = vh::App::Get<vh::Renderer2D>();
        CHECK(renderer);

        if (mActiveView->mRootWidget)
        {
            mActiveView->mRootWidget->SetSize(renderer->GetWidth(), renderer->GetHeight());
        }
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

            if (mActiveView && mActiveView->mRootWidget)
            {
                mActiveView->mRootWidget->OnClick(x, y);
            }
        }
    }
    if (mFocused)
    {
        mFocused->HandleEvent(event);
    }
}

void GUI2D::SetFocus(Widget* widget)
{
    if (mFocused != nullptr) mFocused->OnBlur();

    mFocused = widget;

    if (mFocused != nullptr) mFocused->OnFocus();
}

}
