#pragma once

#include "Modules/Renderer/RendererOptions.hpp"

namespace vh
{
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
