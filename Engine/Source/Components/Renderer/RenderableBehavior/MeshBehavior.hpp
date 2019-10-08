#pragma once

#include "Renderable/Mesh.hpp"
#include "RenderableBehavior/RenderableBehavior.hpp"
#include "Utils/Property.hpp"

namespace vh
{

class MeshBehavior : public RenderableBehavior<eRenderBlockType::Mesh, Mesh>
{
    VH_PROPERTY_RW(bool, IgnoreDepth);

public:
    MeshBehavior(const char* name);

    virtual void SetupPayload(Mesh::Payload* payload) override;

    virtual uint16_t GetFlags() override
    {
        return eRenderBlockFlags::Active | (mInterpolated ? eRenderBlockFlags::Interpolated : 0x0);
    }

    // -- indicates whether the mesh was loaded successfully
    bool IsValid();

    void SetInterpolated(bool interpolated)
    {
        mInterpolated = interpolated;
    }

private:
    glm::mat4 mModel;
    bool mInterpolated = true;
};

} // namespace vh
