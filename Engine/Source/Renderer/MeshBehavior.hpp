#pragma once

#include "Actor/ActorBehavior.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderable/Mesh.hpp"
#include "Renderable/RenderableBehavior.hpp"
#include "Utils/Property.hpp"

namespace vh
{

class MeshBehavior : public RenderableBehavior<eRenderBlockType::Mesh, Mesh>
{
    VH_PROPERTY_RW(bool, IgnoreDepth);

public:
    MeshBehavior(const char* name);

    virtual void SetupPayload(Mesh::Payload* payload) override;

    // -- indicates whether the mesh was loaded successfully
    bool IsValid();

private:
    glm::mat4 mModel;
};

} // namespace vh
