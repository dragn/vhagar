#pragma once

#include "Modules/Renderer/RenderableBehavior/RenderableBehavior.hpp"
#include "Modules/standard.hpp"

namespace vh
{
    /*
          //      \\
        BBBB Mesh BBBB
          \\      //

        Mesh rendering behavior.
    */

    class BMesh : public ActorBehavior
    {
        VH_PROPERTY_RW(bool, IgnoreDepth);
        VH_PROPERTY_RW(std::string, Path);

    public:
        BMesh() = default;
        BMesh(const char* path) : mPath(path), mIgnoreDepth(false) {}
    };

} // namespace vh
