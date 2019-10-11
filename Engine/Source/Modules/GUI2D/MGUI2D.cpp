#include "Modules/VhModules_PCH.hpp"
#include "MGUI2D.hpp"

#include "Modules/Renderer2D/MRenderer2D.hpp"
#include "Modules/ResourceSystem/MResourceSystem.hpp"

using namespace vh;

namespace gui
{

VH_MODULE_IMPL(MGUI2D);

vh::Ret MGUI2D::TickInit(uint32_t delta)
{
    WAIT_REQUIRED(MRenderer2D);
    WAIT_REQUIRED(MResourceSystem);

    mRenderer = App::Get<MRenderer2D>();
    CHECK(mRenderer);

    mScale = mRenderer->GetScale();

    mArrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    mBeamCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    mHandCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    return Ret::SUCCESS;
}

vh::Ret MGUI2D::TickRun(uint32_t delta)
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
        MRenderer2D* mRenderer = App::Get<MRenderer2D>();
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

vh::Ret MGUI2D::TickClose(uint32_t delta)
{
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

void MGUI2D::SetView(View* view, bool withFade)
{
    if (IsInTransition()) return;

    if (withFade) StartTransition();
    
    SafeDelete(mNextView);
    mNextView = view;
}

void MGUI2D::HandleEvent(SDL_Event* event)
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

TTF_Font* MGUI2D::GetFont() const
{
    return App::Get<MResourceSystem>()->GetResource<RFont>(mOptions.fontPath)->GetFont(mOptions.fontSize);
}

TTF_Font* MGUI2D::GetHdr1Font() const
{
    return App::Get<MResourceSystem>()->GetResource<RFont>(mOptions.hdr1FontPath)->GetFont(mOptions.hdr1FontSize);
}

TTF_Font* MGUI2D::GetHdr2Font() const
{
    return App::Get<MResourceSystem>()->GetResource<RFont>(mOptions.hdr2FontPath)->GetFont(mOptions.hdr2FontSize);
}

void MGUI2D::FocusedDestroyed(Widget* widget)
{
    CHECK(widget == mFocused);

    mFocused = nullptr;
}

void MGUI2D::SetFocus(Widget* widget)
{
    if (mFocused != nullptr)
    {
        mFocused->mFocused = false;
        mFocused->OnBlurInternal();
        mFocused->OnDestroy.Remove(this, &MGUI2D::FocusedDestroyed);
    }

    mFocused = widget;

    if (mFocused != nullptr)
    {
        mFocused->mFocused = true;
        mFocused->OnFocusInternal();
        mFocused->OnDestroy.Add(this, &MGUI2D::FocusedDestroyed);
    }
}

SDL_Cursor* MGUI2D::GetArrowCursor()
{
    return mArrowCursor;
}

SDL_Cursor* MGUI2D::GetHandCursor()
{
    return mHandCursor;
}

SDL_Cursor* MGUI2D::GetBeamCursor()
{
    return mBeamCursor;
}

void MGUI2D::SetModalView(View* view, eTransitionMode mode)
{
    if (mode == eTransitionMode::FADE) StartTransition();

    SafeDelete(mNextModalView);
    mNextModalView = view;
}

void MGUI2D::Back(eTransitionMode mode)
{
    if (mode == eTransitionMode::FADE) StartTransition();

    mGoBack = true;
}

void MGUI2D::BackToMain(eTransitionMode mode)
{
    if (mode == eTransitionMode::FADE) StartTransition();

    mGoBackToMain = true;
}

void MGUI2D::CalcTextSize(const char* text, int32_t& outWidth, int32_t& outHeight)
{
    MRenderer2D* mRenderer = App::Get<MRenderer2D>();
    mRenderer->CalcTextSize(GetFont(), text, outWidth, outHeight);
}

void MGUI2D::ApplyTransition()
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

bool MGUI2D::WaitForTransition()
{
    if (!IsInTransition()) return true;

    // wait for transitioning half way
    return mTransition >= 0x100;
}

bool MGUI2D::IsInTransition()
{
    return mTransition >= 0;
}

void MGUI2D::StartTransition()
{
    mTransition = mView == nullptr ? 0x100 : 0x0;
}

}
