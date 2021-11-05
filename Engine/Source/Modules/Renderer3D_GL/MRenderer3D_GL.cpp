#include "Modules/VhModules_PCH.hpp"
#include "MRenderer3D_GL.hpp"

#include "Modules/PlayerController/MPlayerController.hpp"
#include "Modules/Renderer3D_GL/Behaviors/BMesh_GL.hpp"
#include "Modules/Renderer3D_GL/Behaviors/BSkyBox_GL.hpp"
#include "Modules/Renderer3D_GL/Behaviors/BPointLight_GL.hpp"
#include "Modules/Renderer3D_GL/Behaviors/BOverlay_GL.hpp"
#include "Modules/World/CameraBehavior.hpp"

#include "Utils/imgui/imgui_impl_sdl.h"

namespace vh
{
    vh::Ret MRenderer3D_GL::TickInit(uint32_t delta)
    {
        if (mRenderThread.IsStarted())
        {
            // Wait for render thread to initialize
            return mRenderThread.IsReady() ? Ret::SUCCESS : Ret::CONTINUE;
        }

        // -- Actor Behavior overrides
        ActorBehavior::AddOverride<BMesh, BMesh_GL>();
        ActorBehavior::AddOverride<BSkyBox, BSkyBox_GL>();
        ActorBehavior::AddOverride<BPointLight, BPointLight_GL>();
        ActorBehavior::AddOverride<BOverlay, BOverlay_GL>();
        // --

        if (InitSDL() == Ret::FAILURE)
        {
            return Ret::FAILURE;
        }

        mRenderThread.Start(mWindow, GetOptions());

        return Ret::CONTINUE;
    }

    vh::Ret MRenderer3D_GL::TickRun(uint32_t delta)
    {
        return Ret::CONTINUE;
    }

    Ret MRenderer3D_GL::InitSDL()
    {
        LOG(INFO) << "SDL Initialization";

        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            LOG(FATAL) << SDL_GetError();
            return Ret::FAILURE;
        }

        Uint32 flags = SDL_WINDOW_OPENGL;

        if (GetOptions().borderless) flags |= SDL_WINDOW_BORDERLESS;

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        if (GetOptions().antialias != RendererOptions::AA_OFF)
        {
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, int(GetOptions().antialias) << 2);
        }

        // Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
        mWindow = SDL_CreateWindow(
            "GameEngine Demo",
            SDL_WINDOWPOS_CENTERED_DISPLAY(GetOptions().monitor),
            SDL_WINDOWPOS_CENTERED_DISPLAY(GetOptions().monitor),
            GetOptions().screenWidth,
            GetOptions().screenHeight,
            flags);

        if (mWindow == nullptr)
        {
            LOG(FATAL) << "SDL create windows error: " << SDL_GetError();
        }

        // Set relative mouse mode
        SDL_SetRelativeMouseMode(SDL_TRUE);

        return Ret::SUCCESS;
    }

    vh::Ret MRenderer3D_GL::TickClose(uint32_t delta)
    {
        mRenderThread.Stop();

        SDL_DestroyWindow(mWindow);
        SDL_Quit();

        // -- Release behavior overrides
        ActorBehavior::RemoveOverride<BMesh>();
        ActorBehavior::RemoveOverride<BSkyBox>();
        ActorBehavior::RemoveOverride<BPointLight>();
        ActorBehavior::RemoveOverride<BOverlay>();
        // --

        return Ret::SUCCESS;
    }

    void MRenderer3D_GL::StartFrame()
    {
        CHECK(IsRunning());

        RenderBuffer& buffer = mRenderThread.GetWriteBuffer();
        buffer.header.size = 0;
        buffer.header.time = SDL_GetTicks();
        buffer.header.timestep = 16;

        MPlayerController* controller = App::Get<MPlayerController>();
        if (controller && !controller->GetControlledActor().expired())
        {
            CameraBehavior* camera = controller->GetControlledActor().lock()->GetBehaviorOfType<CameraBehavior>();
            if (camera)
            {
                buffer.header.view = camera->GetView();
            }
        }
    }

    void MRenderer3D_GL::EndFrame()
    {
        CHECK(IsRunning());

        mRenderThread.FlipBuffers();
    }

    void MRenderer3D_GL::HandleEvent(SDL_Event* event)
    {
        ImGui_ImplSDL2_ProcessEvent(event);
    }

    vh::RenderBuffer& MRenderer3D_GL::GetWriteBuffer()
    {
        return mRenderThread.GetWriteBuffer();
    }

    void MRenderer3D_GL::LoadRes(std::shared_ptr<GLResource> const& res)
    {
        mRenderThread.LoadRes(res);
    }

    void MRenderer3D_GL::UnloadRes(std::shared_ptr<GLResource> const& res)
    {
        mRenderThread.UnloadRes(res);
    }

};
