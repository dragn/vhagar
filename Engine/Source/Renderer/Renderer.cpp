#include "Renderer.hpp"

#include "Common.hpp"
#include "App/App.hpp"
#include "Console/ConsoleCommands.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace vh
{

namespace
{
void reportGLError(int error)
{
    switch (error)
    {
    case GL_NO_ERROR:
        return;
    case GL_INVALID_OPERATION:
        LOG(ERROR) << "GL ERROR: INVALID_OPERATION";
        return;
    case GL_INVALID_VALUE:
        LOG(ERROR) << "GL ERROR: INVALID_VALUE";
        return;
    default:
        LOG(ERROR) << "GL ERROR: " << error;
    }
}
}

DEFINE_COMMAND(toggle_wireframe)
{
    Renderer* renderer = App::Get<Renderer>();
    CHECK(renderer);

    renderer->Toggle(RenderFlags::DRAW_WIREFRAMES);
}

void Renderer::TickInit(uint32_t delta)
{
    LOG(INFO) << "SDL Initialization";
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        LOG(FATAL) << SDL_GetError();
        Close();
        return;
    }

    if (TTF_Init() < 0)
    {
        LOG(FATAL) << TTF_GetError();
        Close();
        return;
    }

    Uint32 flags = SDL_WINDOW_OPENGL;

    if (mOptions.borderless) flags |= SDL_WINDOW_BORDERLESS;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if (mOptions.antialias != RendererOptions::AA_OFF)
    {
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, int(mOptions.antialias) << 2);
    }

    // Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
    mWindow = SDL_CreateWindow(
        "GameEngine Demo",
        SDL_WINDOWPOS_CENTERED_DISPLAY(mOptions.monitor),
        SDL_WINDOWPOS_CENTERED_DISPLAY(mOptions.monitor),
        mOptions.screenWidth,
        mOptions.screenHeight,
        flags);

    if (mWindow == nullptr)
    {
        LOG(FATAL) << "SDL create windows error: " << SDL_GetError();
    }

    mWindowID = SDL_GetWindowID(mWindow);

    // Set relative mouse mode
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Create an OpenGL context associated with the mWindow.
    mGLContext = SDL_GL_CreateContext(mWindow);

    if (mGLContext == nullptr)
    {
        LOG(FATAL) << "GL context create error: " << SDL_GetError();
    }

    LOG(INFO) << "Initializing OpenGL renderer";
    LOG(INFO) << "OpenGL Version: " << glGetString(GL_VERSION);
    LOG(INFO) << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        LOG(FATAL) << "Glew Error: " << glewGetErrorString(err);
        Close();
        GetApp()->Close();
        return;
    }

    float aspect = static_cast<float>(mOptions.screenWidth) / static_cast<float>(mOptions.screenHeight);

    mProjection = glm::infinitePerspective(45.f, aspect, 0.1f);

    if (!GLEW_VERSION_3_0)
    {
        LOG(ERROR) << "Only OpenGL versions 3.0+ supported. Sorry.";
        Close();
        GetApp()->Close();
        return;
    }

    if (SDL_GL_SetSwapInterval(1) != 0)
    {
        LOG(WARNING) << "Can't set swap interval: " << SDL_GetError();
    }

    // Enable backface culling
    glEnable(GL_CULL_FACE);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LEQUAL);

    // Enable cube map textures
    glEnable(GL_TEXTURE_CUBE_MAP);

    // Enable multisampling
    if (mOptions.antialias != RendererOptions::AA_OFF) glEnable(GL_MULTISAMPLE);

    REGISTER_COMMAND(toggle_wireframe);

    FinishInit();
}

void Renderer::TickClose(uint32_t delta)
{
    for (Renderable *obj : mObjects)
    {
        obj->AfterRender();
        delete obj;
    }
    mObjects.clear();

    SDL_GL_DeleteContext(mGLContext);
    SDL_DestroyWindow(mWindow);

    TTF_Quit();
    SDL_Quit();

    FinishClose();
}

void Renderer::AddObject(Renderable *object)
{
    CHECK(IsRunning()) << "Invalid state";
    CHECK(object);

    mObjects.push_front(object);
    object->BeforeRender();
}

void Renderer::RemoveObject(Renderable *object)
{
    CHECK(IsRunning()) << "Invalid state";
    CHECK(object);

    object->AfterRender();
    mObjects.remove(object);
}

void Renderer::TickRun(uint32_t delta)
{
    BeforeRender();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Renderable *obj : mObjects)
    {
        obj->Render(mProjection, mView, this);
    }

    int error = glGetError();
    if (error) reportGLError(error);

    SDL_GL_SwapWindow(mWindow);

    AfterRender();
}

void Renderer::AddLight(const PointLight* light)
{
    if (mLights.size() >= MAX_POINT_LIGHTS)
    {
        LOG(WARNING) << "Point lights limit reached";
        return;
    }
    mLights.push_back(light);
}

void Renderer::RemoveLight(const PointLight* light)
{
    for (auto l = mLights.begin(); l != mLights.end();)
    {
        if (*l == light) l = mLights.erase(l);
        else ++l;
    }
}

const std::vector<const PointLight*> Renderer::GetPointLights() const
{
    return mLights;
}

} // namespace vh
