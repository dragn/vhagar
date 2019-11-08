#pragma once

#include "Modules/Renderer/Renderable/Mesh.hpp"
#include "Modules/Renderer/RenderableBehavior/RenderableBehavior.hpp"
#include "Modules/standard.hpp"

namespace vh
{

    class MeshBehavior : public RenderableBehavior
    {
        VH_PROPERTY_RW(bool, IgnoreDepth);
        VH_PROPERTY_RW(std::shared_ptr<Mesh>, Mesh);

    public:
        MeshBehavior(const char* path);
    };

} // namespace vh
