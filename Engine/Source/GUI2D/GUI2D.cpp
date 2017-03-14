#include "Common.hpp"
#include "GUI2D.hpp"
#include "App/App.hpp"
#include "Renderer2D/Renderer2D.hpp"

using namespace vh;

namespace gui
{

void GUI2D::TickInit(uint32_t delta)
{
    App::CheckRequired<Renderer2D>();

    TTF_Init();
    CHECK(TTF_WasInit()) << "Error initialising SDL_ttf";

    mFont = TTF_OpenFont(mOptions.fontPath, mOptions.fontSize);
    CHECK(mFont) << "Could not open font: " << mOptions.fontPath;

    mHdr1Font = TTF_OpenFont(mOptions.hdr1FontPath, mOptions.hdr1FontSize);
    CHECK(mHdr1Font) << "Could not open font: " << mOptions.hdr1FontPath;

    mHdr2Font = TTF_OpenFont(mOptions.hdr2FontPath, mOptions.hdr2FontSize);
    CHECK(mHdr2Font) << "Could not open font: " << mOptions.hdr2FontPath;

    Renderer2D* renderer = App::Get<Renderer2D>();
    CHECK(renderer);
    mScale = renderer->GetScale();

    mArrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    mBeamCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    mHandCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    FinishInit();
}

void GUI2D::TickRun(uint32_t delta)
{
    if (!mModalViewStack.empty())
    {
        mModalViewStack.back()->Render();
    }
    else if (mView != nullptr)
    {
        mView->Render();
    }

    if (mNextView != nullptr)
    {
        /* clear focus */
        SetFocus(nullptr);

        // switch to new view
        SafeDelete(mView);
        std::swap(mView, mNextView);

        // clear modal view stack
        for (View* view : mModalViewStack) SafeDelete(view);
        mModalViewStack.clear();

        // remove requested next modal view
        SafeDelete(mNextModalView);

        // stretch root widget to all available space
        Renderer2D* renderer = App::Get<Renderer2D>();
        CHECK(renderer);

        if (mView->mRootWidget)
        {
            mView->mRootWidget->SetSize(renderer->GetWidth(), renderer->GetHeight());
        }
    }

    if (mNextModalView != nullptr)
    {
        /* clear focus */
        SetFocus(nullptr);

        // push new view to stack
        mModalViewStack.push_back(mNextModalView);

        // stretch root widget to all available space
        Renderer2D* renderer = App::Get<Renderer2D>();
        CHECK(renderer);

        if (mNextModalView->mRootWidget)
        {
            mNextModalView->mRootWidget->SetSize(renderer->GetWidth(), renderer->GetHeight());
        }

        mNextModalView = nullptr;
    }

    if (mGoBack)
    {
        mGoBack = false;

        // pop view from stack
        SafeDelete(mModalViewStack.back());
        mModalViewStack.pop_back();

        // set dirty flag to redraw next view
        if (mModalViewStack.empty())
        {
            mView->mRootWidget->SetDirty();
        }
        else
        {
            mModalViewStack.back()->mRootWidget->SetDirty();
        }
    }

    if (mGoBackToMain)
    {
        mGoBackToMain = false;

        // clear modal view stack
        for (View* view : mModalViewStack) SafeDelete(view);
        mModalViewStack.clear();

        // set dirty flag to main view
        if (mView && mView->mRootWidget) mView->mRootWidget->SetDirty();
    }
}

void GUI2D::TickClose(uint32_t delta)
{
    if (mFont) TTF_CloseFont(mFont);

    if (TTF_WasInit()) TTF_Quit();

    SafeDelete(mView);
    SafeDelete(mNextView);
    for (View* view: mModalViewStack) SafeDelete(view);
    mModalViewStack.clear();
    SafeDelete(mNextModalView);

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
    SafeDelete(mNextView);
    mNextView = view;
}

void GUI2D::HandleEvent(SDL_Event* event)
{
    View* view = mModalViewStack.empty() ? mView : mModalViewStack.back();

    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            int32_t x = event->button.x / mScale;
            int32_t y = event->button.y / mScale;

            if (view && view->mRootWidget)
            {
                view->mRootWidget->OnClickInternal(x, y);
            }
        }
    }
    if (event->type == SDL_MOUSEMOTION)
    {
        int32_t x = event->motion.x / mScale;
        int32_t y = event->motion.y / mScale;

        if (view && view->mRootWidget)
        {
            view->mRootWidget->OnMouseMove(x, y);
        }
    }
    if (mFocused)
    {
        mFocused->HandleEvent(event);
    }
}

void GUI2D::FocusedDestroyed(Widget* widget)
{
    CHECK(widget == mFocused);

    mFocused = nullptr;
}

void GUI2D::SetFocus(Widget* widget)
{
    if (mFocused != nullptr)
    {
        mFocused->mFocused = false;
        mFocused->OnBlurInternal();
        mFocused->OnDestroy.Remove(this, &GUI2D::FocusedDestroyed);
    }

    mFocused = widget;

    if (mFocused != nullptr)
    {
        mFocused->mFocused = true;
        mFocused->OnFocusInternal();
        mFocused->OnDestroy.Add(this, &GUI2D::FocusedDestroyed);
    }
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

void GUI2D::SetModalView(View* view)
{
    SafeDelete(mNextModalView);
    mNextModalView = view;
}

void GUI2D::Back()
{
    mGoBack = true;
}

void GUI2D::BackToMain()
{
    mGoBackToMain = true;
}

}
