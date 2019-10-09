#include "Modules/VhModules_PCH.hpp"
#include "GUI2D.hpp"

#include "Modules/Renderer2D/Renderer2D.hpp"

using namespace vh;

namespace gui
{

VH_MODULE_IMPL(GUI2D);

vh::Ret GUI2D::TickInit(uint32_t delta)
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

    mRenderer = App::Get<Renderer2D>();
    CHECK(mRenderer);

    mScale = mRenderer->GetScale();

    mArrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    mBeamCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    mHandCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    return Ret::SUCCESS;
}

vh::Ret GUI2D::TickRun(uint32_t delta)
{
    ApplyTransition();

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
        if (!WaitForTransition()) return Ret::CONTINUE;

        /* clear focus */
        SetFocus(nullptr);

        // switch to new view
        SafeDelete(mView);
        std::swap(mView, mNextView);

        // clear modal view stack
        for (View* view : mModalViewStack) SafeDelete(view);
        mModalViewStack.clear();

        // stretch root widget to all available space
        if (mView->mRootWidget)
        {
            mView->mRootWidget->SetSize(mRenderer->GetWidth(), mRenderer->GetHeight());
        }

        mView->OnDestroy.Add([this]
        {
            // clear modal view stack on view destruction
            for (View* view : mModalViewStack) SafeDelete(view);
            mModalViewStack.clear();
        });

        return Ret::CONTINUE;
    }

    if (mNextModalView != nullptr)
    {
        if (!WaitForTransition()) return Ret::CONTINUE;

        /* clear focus */
        SetFocus(nullptr);

        // push new view to stack
        mModalViewStack.push_back(mNextModalView);

        // stretch root widget to all available space
        Renderer2D* mRenderer = App::Get<Renderer2D>();
        CHECK(mRenderer);

        if (mNextModalView->mRootWidget)
        {
            mNextModalView->mRootWidget->SetSize(mRenderer->GetWidth(), mRenderer->GetHeight());
        }

        mNextModalView = nullptr;

        return Ret::CONTINUE;
    }

    if (mGoBack)
    {
        if (!WaitForTransition()) return Ret::CONTINUE;

        mGoBack = false;

        // pop view from stack
        if (!mModalViewStack.empty())
        {
            SafeDelete(mModalViewStack.back());
            mModalViewStack.pop_back();
        }

        // set dirty flag to redraw next view
        if (mModalViewStack.empty())
        {
            mView->mRootWidget->SetDirty();
        }
        else
        {
            mModalViewStack.back()->mRootWidget->SetDirty();
        }

        return Ret::CONTINUE;
    }

    if (mGoBackToMain)
    {
        if (!WaitForTransition()) return Ret::CONTINUE;

        mGoBackToMain = false;

        // clear modal view stack
        for (View* view : mModalViewStack) SafeDelete(view);
        mModalViewStack.clear();

        // set dirty flag to main view
        if (mView && mView->mRootWidget) mView->mRootWidget->SetDirty();

        return Ret::CONTINUE;
    }

    return Ret::CONTINUE;
}

vh::Ret GUI2D::TickClose(uint32_t delta)
{
    if (mFont) TTF_CloseFont(mFont);

    if (TTF_WasInit()) TTF_Quit();

    SafeDelete(mView);
    SafeDelete(mNextView);
    for (View* view : mModalViewStack) SafeDelete(view);
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

    return Ret::SUCCESS;
}

void GUI2D::SetView(View* view, bool withFade)
{
    if (IsInTransition()) return;

    if (withFade) StartTransition();
    
    SafeDelete(mNextView);
    mNextView = view;
}

void GUI2D::HandleEvent(SDL_Event* event)
{
    if (IsInTransition()) return;

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

void GUI2D::SetModalView(View* view, eTransitionMode mode)
{
    if (mode == eTransitionMode::FADE) StartTransition();

    SafeDelete(mNextModalView);
    mNextModalView = view;
}

void GUI2D::Back(eTransitionMode mode)
{
    if (mode == eTransitionMode::FADE) StartTransition();

    mGoBack = true;
}

void GUI2D::BackToMain(eTransitionMode mode)
{
    if (mode == eTransitionMode::FADE) StartTransition();

    mGoBackToMain = true;
}

void GUI2D::CalcTextSize(const char* text, int32_t& outWidth, int32_t& outHeight)
{
    Renderer2D* mRenderer = App::Get<Renderer2D>();
    mRenderer->CalcTextSize(mFont, text, outWidth, outHeight);
}

void GUI2D::ApplyTransition()
{
    if (mTransition >= 0x0 && mTransition < 0x200)
    {
        // fade transition
        mRenderer->SetAlpha(mTransition < 0x100 ? 0xff - mTransition : mTransition - 0x100);
        mTransition += 0x10;
    }

    if (mTransition >= 0x200)
    {
        // transition finished
        mTransition = -1;
        mRenderer->SetAlpha(SDL_ALPHA_OPAQUE);
    }
}

bool GUI2D::WaitForTransition()
{
    if (!IsInTransition()) return true;

    // wait for transitioning half way
    return mTransition >= 0x100;
}

bool GUI2D::IsInTransition()
{
    return mTransition >= 0;
}

void GUI2D::StartTransition()
{
    mTransition = mView == nullptr ? 0x100 : 0x0;
}

}
