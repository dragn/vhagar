#include "Modules/VhModules_PCH.hpp"
#include "MRenderer3D_GL.hpp"

#include "Modules/PlayerController/MPlayerController.hpp"
#include "Modules/Renderer3D_GL/Behaviors/BMesh_GL.hpp"
#include "Modules/World/CameraBehavior.hpp"

namespace vh
{
    vh::Ret MRenderer3D_GL::TickInit(uint32_t delta)
    {
        if (mRenderThread.IsStarted())
        {
            // Wait for render thread to initialize
            return mRenderThread.IsReady() ? Ret::SUCCESS : Ret::CONTINUE;
        }

        ActorBehavior::AddOverride<BMesh, BMesh_GL>();

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

        mWindowID = SDL_GetWindowID(mWindow);

        // Set relative mouse mode
        SDL_SetRelativeMouseMode(SDL_TRUE);

        mRenderThread.Start(mWindow, GetOptions());

        return Ret::CONTINUE;
    }

    vh::Ret MRenderer3D_GL::TickClose(uint32_t delta)
    {
        mRenderThread.Stop();

        SDL_DestroyWindow(mWindow);

        SDL_Quit();

        ActorBehavior::RemoveOverride<BMesh>();

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

    vh::RenderBuffer& MRenderer3D_GL::GetWriteBuffer()
    {
        return mRenderThread.GetWriteBuffer();
    }

    /*
    void MRenderer3D_New::TickRenderable(RenderableBehavior* behavior)
    {
        RenderBlock* block = GetWriteBuffer().AllocateNewBlock();
        if (block == nullptr) return;

        block->flags = GetFlags();
        block->type = TYPE;
        behavior->SetupPayload(reinterpret_cast<typename RENDERABLE_TYPE::Payload*>(&block->payload));
    }
    */

};
