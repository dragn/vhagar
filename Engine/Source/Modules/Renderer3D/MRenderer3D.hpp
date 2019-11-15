#pragma once

#include "Modules/Renderer3D/RendererOptions.hpp"

namespace vh
{

    /*
          //            \\
        MMMM Renderer3D MMMM
          \\            //

        Base 3D-renderer module interface.
        Does not provide an actual implementation, which should be provided
        by extension modules (e.g. Renderer3D_GL for OpenGL rendering).
    */
    class MRenderer3D : public Module
    {
        VH_MODULE(MRenderer3D);
        VH_PROPERTY_RO(RendererOptions, Options);

    public:
        MRenderer3D(RendererOptions opts)
            : Module(TickFrequency::NEVER)
            , mOptions(opts)
        {}
    };

} // namespace vh
