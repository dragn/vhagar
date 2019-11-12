#pragma once

#include "Modules/Renderer/RenderableBehavior/RenderableBehavior.hpp"
#include "Modules/standard.hpp"

namespace vh
{

    class BMesh : public RenderableBehavior
    {
        VH_PROPERTY_RW(bool, IgnoreDepth);
        VH_PROPERTY_RW(std::string, Path);

    public:
        BMesh() = default;
        BMesh(const char* path)
            : mPath(path)
        {}
    };

} // namespace vh
