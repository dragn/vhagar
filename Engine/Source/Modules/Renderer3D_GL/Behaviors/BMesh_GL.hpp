#pragma once

#include "Modules/Renderer/RenderableBehavior/BMesh.hpp"
#include "Modules/Renderer3D_GL/ResourceTypes/RMesh_GL.hpp"

namespace vh
{

    class BMesh_GL : public BMesh
    {
    public:
        BMesh_GL();
        virtual ~BMesh_GL();

        virtual void StartPlay() override;
        virtual void EndPlay() override;
        virtual void Tick(uint32_t delta) override;

    private:
        std::shared_ptr<RMesh_GL> mMesh;
    };

} // namespace vh
