#include "SDLRenderer.hpp"

#include "Common.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace vh
{

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

void SDLRenderer::TickInit(uint32_t delta) {
    LOG(INFO) << "SDL Initialization";
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG(FATAL) << SDL_GetError();
        Close();
        return;
    }

    // Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
    mWindow = SDL_CreateWindow(
            "GameEngine Demo", 0, 0,
            mOptions.screenWidth, mOptions.screenHeight,
            SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

    mWindowID = SDL_GetWindowID(mWindow);

    // Set relative mouse mode
    SDL_SetRelativeMouseMode(SDL_TRUE);

    // Create an OpenGL context associated with the mWindow.
    mGLContext = SDL_GL_CreateContext(mWindow);

    LOG(INFO) << "Initializing OpenGL renderer";
    LOG(INFO) << "OpenGL Version: " << glGetString(GL_VERSION);
    LOG(INFO) << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LOG(FATAL) << "Glew Error: " << glewGetErrorString(err);
        Close();
        return;
    }

    mProjection = glm::perspective(45.f, 4.0f / 3.0f, 0.1f, 100.f);

    if (!GLEW_VERSION_3_0) {
        LOG(ERROR) << "Only OpenGL versions 3.0+ supported. Sorry.";
        Close();
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

    int maxSamples;

    glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &maxSamples);
    mMultisample = (maxSamples > 1);
    LOG(INFO) << "Multi-sampling is: " << (mMultisample ? "enabled" : "disabled");

    if (mMultisample) {
        glEnable(GL_MULTISAMPLE);
        // setting up multisampling
        GLuint num_samples = maxSamples >= 8 ? 8 : 1;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_RGBA8, mOptions.screenWidth, mOptions.screenHeight, false);

        glGenTextures(1, &depth_tex);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depth_tex);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, num_samples, GL_DEPTH_COMPONENT, mOptions.screenWidth, mOptions.screenHeight, false);

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, depth_tex, 0);
    }

    FinishInit();
}

void SDLRenderer::TickClose(uint32_t delta) {
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

void SDLRenderer::AddObject(Renderable *object) {
    CHECK(IsRunning()) << "Invalid state";
    CHECK(object);

    mObjects.push_front(object);
    object->beforeRender();
}

void SDLRenderer::RemoveObject(Renderable *object) {
    CHECK(IsRunning()) << "Invalid state";
    CHECK(object);

    object->afterRender();
    mObjects.remove(object);
    delete object;
}

void SDLRenderer::TickRun(uint32_t delta) {
    BeforeRender();

    if (mMultisample) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    }

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_BLEND);
    for (Renderable *obj : mObjects) {
        obj->render(mProjection, mView);
    }
    if (mLightSources.size()) {
        for (Renderable *obj : mObjects) {
            obj->render(mProjection, mView, mLightSources[0]);
        }
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    for (size_t i = 1; i < mLightSources.size(); i++) {
        LightSource light = mLightSources[i];
        for (Renderable *obj : mObjects) {
            obj->render(mProjection, mView, light);
        }
    }

    if (mMultisample) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);   // Make sure no FBO is set as the draw framebuffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo); // Make sure your multisampled FBO is the read framebuffer
        glDrawBuffer(GL_BACK);                       // Set the back buffer as the draw buffer
        glBlitFramebuffer(0, 0, mOptions.screenWidth, mOptions.screenHeight,
                0, 0, mOptions.screenWidth, mOptions.screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    int error = glGetError();
    if (error) reportGLError(error);

    SDL_GL_SwapWindow(mWindow);

    AfterRender();
}

} // namespace vh
