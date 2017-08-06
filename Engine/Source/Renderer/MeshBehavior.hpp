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
public:
    MeshBehavior(Actor* owner, const char* name);

    virtual void SetupPayload(Mesh::Payload* payload) override;

    // -- indicates whether the mesh was loaded successfully
    bool IsValid();

    VH_PROPERTY_RW(glm::vec3, RelPos);
    VH_PROPERTY_RW(glm::quat, RelRot);
    VH_PROPERTY_RW(glm::vec3, Scale);
    VH_PROPERTY_RW(bool, UseOwnerScale);

private:
    glm::mat4 mModel;
};

} // namespace vh
