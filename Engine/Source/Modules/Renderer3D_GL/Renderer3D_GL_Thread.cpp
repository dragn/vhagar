#include "Modules/VhModules_PCH.hpp"
#include "Renderer3D_GL_Thread.hpp"

#include "Modules/Renderer3D_GL/GLBuffers.hpp"

namespace vh
{
    const static size_t MAX_POINT_LIGHTS = 10;
    const static size_t MAX_AMBIENT_LIGHTS = 1;
    const static size_t MAX_DIRECTION_LIGHTS = 10;

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

    void MRenderer3D_GL_Thread::Start(SDL_Window* window, const RendererOptions& options)
    {
        CHECK(!IsStarted());
        CHECK(!IsReady());

        mWindow = window;
        mOptions = options;

        mThread.Start([](void* _this)->int { reinterpret_cast<MRenderer3D_GL_Thread*>(_this)->Run(); return 0; }, this);

        mStarted.store(true);
    }

    void MRenderer3D_GL_Thread::Stop(bool wait /*= true*/)
    {
        if (IsStarted())
        {
            mRequestStop.store(true);
            if (wait)
            {
                mThread.Wait(-1);
            }
        }
    }

    vh::RenderBuffer& MRenderer3D_GL_Thread::GetWriteBuffer()
    {
        return mRenderBuffers.GetNextBuffer();
    }

    void MRenderer3D_GL_Thread::FlipBuffers()
    {
        mRenderBuffers.FlipBuffers();
    }

    void MRenderer3D_GL_Thread::Run()
    {
        DoInit();

        mReady.store(true);

        Utils::GetShaderProgram("SkyDome");

        while (!mRequestStop.load())
        {
            // -- Reset frame
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // -- Process resource loading
            ProcessLoadQueue();

            // -- Main Render Loop
            mRenderBuffers.LockRead();

            uint32_t time = SDL_GetTicks();
            const RenderBuffer& last = mRenderBuffers.GetLastBuffer();
            const RenderBuffer& cur = mRenderBuffers.GetCurrentBuffer();

            CHECK(last.header.time <= time && cur.header.time <= time) << "sanity check failed";
            CHECK(last.header.time <= cur.header.time) << "sanity check failed";
            float factor = (float)(time - cur.header.time) / (cur.header.time - last.header.time);
            if (cur.header.time == last.header.time) factor = 0.0f;
            factor = Math::Clamp(factor, 0.0f, 1.0f);
            DoRender(last, cur, factor);

            mRenderBuffers.UnlockRead();
            // --

            // -- Process resource unloading
            ProcessUnloadQueue();

            // -- Error handling
            int error = glGetError();
            if (error) reportGLError(error);

            // -- Process frame counter
            HandleFrameCount();

            // -- Done. Flip the window.
            SDL_GL_SwapWindow(mWindow);
        }

        SDL_GL_DeleteContext(mGLContext);
    }

    void MRenderer3D_GL_Thread::HandleFrameCount()
    {
        mFrameCount++;

        if (SDL_GetTicks() - mLastFPSReport > 1000)
        {
            mFPS.store(1000.0f * (float)mFrameCount / (SDL_GetTicks() - mLastFPSReport));
            mLastFPSReport = SDL_GetTicks();
            mFrameCount = 0;
        }
    }

    void MRenderer3D_GL_Thread::DoInit()
    {
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
            return;
        }

        float aspect = static_cast<float>(mOptions.screenWidth) / static_cast<float>(mOptions.screenHeight);

        mProjection = glm::infinitePerspective(45.f, aspect, 0.1f);

        if (!GLEW_VERSION_3_0)
        {
            LOG(FATAL) << "Only OpenGL versions 3.0+ supported. Sorry.";
            return;
        }

        if (SDL_GL_SetSwapInterval(mOptions.vsync ? 1 : 0) != 0)
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

        glEnable(GL_DITHER);

        // Enable multisampling
        if (mOptions.antialias != RendererOptions::AA_OFF) glEnable(GL_MULTISAMPLE);
    }

    void MRenderer3D_GL_Thread::DoRender(const RenderBuffer& last, const RenderBuffer& cur, float factor)
    {
        glm::mat4 view = glm::mix(last.header.view, cur.header.view, factor);
        glm::mat4 projection = mProjection;

        // first pass - load/unload, collect lights
        std::vector<GLPointLight::Payload> lights;
        for (size_t idx = 0; idx < cur.header.size; ++idx)
        {
            const RenderBlock& block = cur.blocks[idx];

            // do load task
            if (block.type == eRenderBlockType::RenderableLoad)
            {
                void* ptr = const_cast<void*>(reinterpret_cast<const void*>(block.payload));
                if (ptr != nullptr)
                {
                    reinterpret_cast<GLResource*>(ptr)->AddRef();
                }
            }

            // do release task
            else if (block.type == eRenderBlockType::RendarableRelease)
            {
                void* ptr = const_cast<void*>(reinterpret_cast<const void*>(block.payload));
                if (ptr != nullptr)
                {
                    reinterpret_cast<GLResource*>(ptr)->ReleaseRef();
                }
            }

            // collect lights
            else if (block.type == eRenderBlockType::Light && (block.flags & eRenderBlockFlags::Active))
            {
                if (last.blocks[idx].type == eRenderBlockType::Light)
                {
                    GLPointLight::Payload lastPayload = *reinterpret_cast<const GLPointLight::Payload*>(last.blocks[idx].payload);
                    const GLPointLight::Payload& curPayload = *reinterpret_cast<const GLPointLight::Payload*>(cur.blocks[idx].payload);
                    if (block.flags & eRenderBlockFlags::Interpolated)
                    {
                        lastPayload.pos = glm::mix(lastPayload.pos, curPayload.pos, factor);
                    }
                    lights.push_back(lastPayload);
                }
            }
        }

        // second pass - render skyboxes
        for (size_t idx = 0; idx < cur.header.size; ++idx)
        {
            const RenderBlock& block = cur.blocks[idx];

            if (block.type == eRenderBlockType::SkyBox && (block.flags & eRenderBlockFlags::Active))
            {
                const GLSkyBox::Payload& payload = *reinterpret_cast<const GLSkyBox::Payload*>(block.payload);
                DoRenderSkyBox(view, projection, payload);
            }
        }

        // second pass - render meshes
        for (size_t idx = 0; idx < last.header.size; ++idx)
        {
            const RenderBlock& block = last.blocks[idx];

            // do mesh render task
            if (block.type == eRenderBlockType::Mesh && (block.flags & eRenderBlockFlags::Active))
            {
                // skip depth-ignore meshes
                if (reinterpret_cast<const GLMesh::Payload*>(block.payload)->ignoreDepth) continue;

                GLMesh::Payload lastPayload = *reinterpret_cast<const GLMesh::Payload*>(block.payload);
                const GLMesh::Payload& curPayload = *reinterpret_cast<const GLMesh::Payload*>(cur.blocks[idx].payload);
                if (block.flags & eRenderBlockFlags::Interpolated)
                {
                    if (lastPayload.owner == curPayload.owner)
                    {
                        lastPayload.translate = glm::mix(lastPayload.translate, curPayload.translate, factor);
                        lastPayload.scale = glm::mix(lastPayload.scale, curPayload.scale, factor);
                        lastPayload.rotate = glm::slerp(lastPayload.rotate, curPayload.rotate, factor);
                    }
                }
                DoRenderMesh(view, projection, &lastPayload, lights);
            }
        }

        // third pass - depth-ignore meshes
        for (size_t idx = 0; idx < last.header.size; ++idx)
        {
            const RenderBlock& block = last.blocks[idx];

            // do mesh render task
            if (block.type == eRenderBlockType::Mesh && (block.flags & eRenderBlockFlags::Active))
            {
                // render only depth-ignore meshes
                if (!(reinterpret_cast<const GLMesh::Payload*>(block.payload)->ignoreDepth)) continue;

                GLMesh::Payload lastPayload = *reinterpret_cast<const GLMesh::Payload*>(block.payload);
                const GLMesh::Payload& curPayload = *reinterpret_cast<const GLMesh::Payload*>(cur.blocks[idx].payload);
                if (block.flags & eRenderBlockFlags::Interpolated)
                {
                    if (lastPayload.owner == curPayload.owner)
                    {
                        lastPayload.translate = glm::mix(lastPayload.translate, curPayload.translate, factor);
                        lastPayload.scale = glm::mix(lastPayload.scale, curPayload.scale, factor);
                        lastPayload.rotate = glm::slerp(lastPayload.rotate, curPayload.rotate, factor);
                    }
                }
                DoRenderMesh(view, projection, &lastPayload, lights);
            }
        }

        // fourth pass - render overlays
        for (size_t idx = 0; idx < cur.header.size; ++idx)
        {
            const RenderBlock& block = cur.blocks[idx];

            if (block.type == eRenderBlockType::Overlay && (block.flags & eRenderBlockFlags::Active))
            {
                const GLOverlay::Payload& payload = *reinterpret_cast<const GLOverlay::Payload*>(block.payload);
                DoRenderOverlay(payload);
            }
        }
    }

    void MRenderer3D_GL_Thread::DoRenderMesh(glm::mat4 view, glm::mat4 projection, const GLMesh::Payload* payload, const std::vector<GLPointLight::Payload>& lights)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.f), payload->translate) * glm::mat4_cast(payload->rotate) * glm::scale(M4(1.f), payload->scale);
        glm::mat4 MVP = projection * view * model;

        const GLBuffers& glInfo = payload->info;

        glUseProgram(payload->progId);

        // get uniform locations
        GLint uidMVP = glGetUniformLocation(payload->progId, "uMVP");
        GLint uidM = glGetUniformLocation(payload->progId, "uM");
        GLint uidV = glGetUniformLocation(payload->progId, "uV");
        GLint uidPLightNum = glGetUniformLocation(payload->progId, "uPLightNum");
        GLint uidPLightPos = glGetUniformLocation(payload->progId, "uPLightPos");
        GLint uidPLightInt = glGetUniformLocation(payload->progId, "uPLightInt");

        // -- setup lights
        glUniform1i(uidPLightNum, static_cast<GLint>(lights.size()));
        V3 lightPos[MAX_POINT_LIGHTS];
        float lightInt[MAX_POINT_LIGHTS];
        for (size_t i = 0; i < lights.size(); ++i)
        {
            lightPos[i] = lights[i].pos;
            lightInt[i] = lights[i].intensity;
        }
        glUniform3fv(uidPLightPos, static_cast<GLsizei>(lights.size()), reinterpret_cast<GLfloat*>(lightPos));
        glUniform1fv(uidPLightInt, static_cast<GLsizei>(lights.size()), reinterpret_cast<GLfloat*>(lightInt));

        // -- setup transforms
        glUniformMatrix4fv(uidMVP, 1, GL_FALSE, reinterpret_cast<float*>(&MVP[0][0]));
        glUniformMatrix4fv(uidM, 1, GL_FALSE, reinterpret_cast<const float*>(&model[0][0]));
        glUniformMatrix4fv(uidV, 1, GL_FALSE, reinterpret_cast<const float*>(&view[0][0]));

        // -- bind texture
        glBindTexture(GL_TEXTURE_2D, glInfo.texture);

        // -- setup buffers
        glEnableVertexAttribArray(0);
        for (GLuint i = 0; i < glInfo.attribCount; i++) glEnableVertexAttribArray(i + 1);

        glBindBuffer(GL_ARRAY_BUFFER, glInfo.attribBuffer);

        // -- vertex positions
        glVertexAttribPointer(0, payload->dim, GL_FLOAT, GL_FALSE, 0, (GLfloat*)0);

        // -- attributes
        GLfloat *offset = 0;
        for (GLuint i = 0; i < glInfo.attribCount; i++)
        {
            glVertexAttribPointer(i + 1, 3, GL_FLOAT, GL_FALSE, 0, offset + payload->vertexCount * (payload->dim + 3 * i));
        }

        if (payload->ignoreDepth)
        {
            glDisable(GL_DEPTH_TEST);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glInfo.indexBuffer);
        glDrawElements(GL_TRIANGLES, glInfo.indexBufferSize, GL_UNSIGNED_INT, (void*)0);

        if (payload->ignoreDepth)
        {
            glEnable(GL_DEPTH_TEST);
        }

        glDisableVertexAttribArray(0);
        for (GLuint i = 0; i < glInfo.attribCount; i++) glDisableVertexAttribArray(i + 1);

        // -- optionally, draw wireframes
        /*
        if (IsOn(RenderFlags::DRAW_WIREFRAMES))
        {
            glUseProgram(mWireProgramID);

            GLint uidWireColor = glGetUniformLocation(mWireProgramID, "uColor");
            GLint uidWireMVP = glGetUniformLocation(mWireProgramID, "uMVP");

            V3 wireColor(0, 0.4f, 0);
            glUniform3fv(uidWireColor, 1, reinterpret_cast<GLfloat*>(&wireColor));
            glUniformMatrix4fv(uidWireMVP, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&MVP[0][0]));

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, glInfo.attribBuffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glInfo.indexBuffer);
            glDrawElements(GL_LINE_STRIP, glInfo.indexBufferSize, GL_UNSIGNED_INT, (void*)0);
            glDisableVertexAttribArray(0);
        }
        */
    }

    void MRenderer3D_GL_Thread::DoRenderSkyBox(glm::mat4 _view, glm::mat4 projection, const GLSkyBox::Payload& payload)
    {
        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);
        glUseProgram(payload.progId);

        M4 view = _view;
        view[3] = { 0, 0, 0, 1 }; // remove translation from view

        M4 MVP = projection * view * glm::scale(M4(1.0f), V3(50.f, 50.f, 50.f));
        Utils::PutUniformMat4(payload.progId, "uMVP", MVP);

        glBindBuffer(GL_ARRAY_BUFFER, payload.info.attribBuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, payload.info.texture);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, payload.info.indexBuffer);
        glDrawElements(GL_TRIANGLES, payload.info.indexBufferSize, GL_UNSIGNED_INT, (void*)0);

        glDisableVertexAttribArray(0);
        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);
    }

    void MRenderer3D_GL_Thread::DoRenderOverlay(const GLOverlay::Payload& payload)
    {
        if (payload.texId == 0) return;

        glDisable(GL_CULL_FACE);
        glUseProgram(payload.progId);

        Utils::PutUniformVec4(payload.progId, "uBounds", payload.bounds);

        if (payload.texId > 0) glBindTexture(GL_TEXTURE_2D, payload.texId);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindBuffer(GL_ARRAY_BUFFER, payload.vertexBuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(0);

        glEnable(GL_CULL_FACE);
    }

    void MRenderer3D_GL_Thread::ProcessLoadQueue()
    {
        cs::CritSectionLock lock(mLoadQueueCS);
        while (!mLoadQueue.empty())
        {
            mLoadQueue.front()->AddRef();
            mLoadQueue.pop();
        }
    }

    void MRenderer3D_GL_Thread::ProcessUnloadQueue()
    {
        cs::CritSectionLock lock(mLoadQueueCS);
        while (!mUnloadQueue.empty())
        {
            mUnloadQueue.front()->ReleaseRef();
            mUnloadQueue.pop();
        }
    }

    void MRenderer3D_GL_Thread::LoadRes(std::shared_ptr<GLResource> const& res)
    {
        cs::CritSectionLock lock(mLoadQueueCS);
        mLoadQueue.push(res);
    }

    void MRenderer3D_GL_Thread::UnloadRes(std::shared_ptr<GLResource> const& res)
    {
        cs::CritSectionLock lock(mLoadQueueCS);
        mUnloadQueue.push(res);
    }

} // namespace vh