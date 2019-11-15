#pragma once

#include "Modules/Renderer/RenderableBehavior/BSkyBox.hpp"
#include "Modules/Renderer3D_GL/GLBuffers.hpp"
#include "Modules/Renderer3D_GL/GLResources/GLSkyBox.hpp"
#include "Modules/Renderer3D_GL/RenderBuffer.hpp"

namespace vh
{

    class BSkyBox_GL : public BSkyBox
    {
    public:
        BSkyBox_GL() = default;
        BSkyBox_GL(const char* filePath) : BSkyBox(filePath) {}

        virtual void StartPlay() override;
        virtual void EndPlay() override;

        virtual void Tick(uint32_t delta) override;

    private:
        std::shared_ptr<GLSkyBox> mGLSkyBox;
    };

} // namespace vh