#pragma once

#include "Modules/Renderer/RenderableBehavior/BMesh.hpp"
#include "Modules/Renderer3D_GL/GLResources/GLMesh.hpp"

namespace vh
{

    class BMesh_GL : public BMesh
    {
    public:
        BMesh_GL() = default;
        BMesh_GL(const char* filePath) : BMesh(filePath) {}

        virtual void StartPlay() override;
        virtual void EndPlay() override;
        virtual void Tick(uint32_t delta) override;

    private:
        std::shared_ptr<GLMesh> mGLMesh;
    };

} // namespace vh
