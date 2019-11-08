#pragma once

namespace vh
{
    class MRenderer3D : public Module
    {
        VH_MODULE(MRenderer3D);

    public:
        MRenderer3D(TickFrequency frequency)
            : Module(frequency)
        {}

        template<typename RENDERABLE_TYPE>
        void TickRenderable(RENDERABLE_TYPE behavior)
        {
            LOG(WARNING) << "Renderable implementation missing: " << VH_STR(RENDERABLE_TYPE);
        }
    };

} // namespace vh
