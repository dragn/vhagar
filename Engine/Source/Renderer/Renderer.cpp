#include "Renderer.hpp"

#include "Common.hpp"
#include "App/App.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace vh
{

const char* Renderer::COMPONENT_NAME = "RENDER";

namespace {
void reportGLError(int error) {
    switch (error) {
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

void Renderer::TickInit(uint32_t delta) {
    LOG(INFO) << "SDL Initialization";
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG(FATAL) << SDL_GetError();
        Close();
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    // Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
    mWindow = SDL_CreateWindow(
            "GameEngine Demo",
            SDL_WINDOWPOS_CENTERED_DISPLAY(1),
            SDL_WINDOWPOS_CENTERED_DISPLAY(1),
            mOptions.screenWidth,
            mOptions.screenHeight,
            SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);

    mWindowID = SDL_GetWindowID(mWindow);

    // Set relative mouse mode
    // SDL_SetRelativeMouseMode(SDL_TRUE);

    // Create an OpenGL context associated with the mWindow.
    mGLContext = SDL_GL_CreateContext(mWindow);

    LOG(INFO) << "Initializing OpenGL renderer";
    LOG(INFO) << "OpenGL Version: " << glGetString(GL_VERSION);
    LOG(INFO) << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LOG(FATAL) << "Glew Error: " << glewGetErrorString(err);
        Close();
        GetApp()->Close();
        return;
    }

    mProjection = glm::perspective(45.f, 4.0f / 3.0f, 0.1f, 100.f);

    if (!GLEW_VERSION_3_0) {
        LOG(ERROR) << "Only OpenGL versions 3.0+ supported. Sorry.";
        Close();
        GetApp()->Close();
        return;
    }

    if (SDL_GL_SetSwapInterval(1) != 0) {
        LOG(WARNING) << "Can't set swap interval: " << SDL_GetError();
    }

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_TEXTURE_CUBE_MAP);
    glEnable(GL_MULTISAMPLE);

    FinishInit();
}

void Renderer::TickClose(uint32_t delta) {
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &tex);
    glDeleteTextures(1, &depth_tex);

    for (Renderable *obj : mObjects) {
        obj->afterRender();
        delete obj;
    }
    mObjects.clear();

    SDL_GL_DeleteContext(mGLContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();

    FinishClose();
}

void Renderer::AddObject(Renderable *object) {
    CHECK(IsRunning()) << "Invalid state";
    CHECK(object);

    mObjects.push_front(object);
    object->beforeRender();
}

void Renderer::RemoveObject(Renderable *object) {
    CHECK(IsRunning()) << "Invalid state";
    CHECK(object);

    object->afterRender();
    mObjects.remove(object);
}

void Renderer::TickRun(uint32_t delta) {
    BeforeRender();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_BLEND);
    for (Renderable *obj : mObjects) {
        obj->render(mProjection, mView);
    }
    if (mLights.size()) {
        for (Renderable *obj : mObjects) {
            obj->render(mProjection, mView, mLights[0]);
        }
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    for (size_t i = 1; i < mLights.size(); i++) {
        Light light = mLights[i];
        for (Renderable *obj : mObjects) {
            obj->render(mProjection, mView, light);
        }
    }

    int error = glGetError();
    if (error) reportGLError(error);

    SDL_GL_SwapWindow(mWindow);

    AfterRender();
}

} // namespace vh
