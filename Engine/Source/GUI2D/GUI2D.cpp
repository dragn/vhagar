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

    mHdr1Font = TTF_OpenFont(mOptions.hdr1FontPath, mOptions.hdr1FontSize);
    CHECK(mHdr1Font) << "Could not open font: " << mOptions.hdr1FontPath;

    mHdr2Font = TTF_OpenFont(mOptions.hdr2FontPath, mOptions.hdr2FontSize);
    CHECK(mHdr2Font) << "Could not open font: " << mOptions.hdr2FontPath;

    vh::Renderer2D* renderer = vh::App::Get<vh::Renderer2D>();
    CHECK(renderer);
    mScale = renderer->GetScale();

    mArrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    mBeamCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    mHandCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

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

    if (mArrowCursor)
    {
        SDL_FreeCursor(mArrowCursor);
        mArrowCursor = nullptr;
    }

    if (mBeamCursor)
    {
        SDL_FreeCursor(mBeamCursor);
        mBeamCursor = nullptr;
    }

    if (mHandCursor)
    {
        SDL_FreeCursor(mHandCursor);
        mHandCursor = nullptr;
    }

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
    if (event->type == SDL_MOUSEMOTION)
    {
        int32_t x = event->motion.x / mScale;
        int32_t y = event->motion.y / mScale;

        if (mActiveView && mActiveView->mRootWidget)
        {
            mActiveView->mRootWidget->OnMouseMove(x, y);
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

SDL_Cursor* GUI2D::GetArrowCursor()
{
    return mArrowCursor;
}

SDL_Cursor* GUI2D::GetHandCursor()
{
    return mHandCursor;
}

SDL_Cursor* GUI2D::GetBeamCursor()
{
    return mBeamCursor;
}

}
