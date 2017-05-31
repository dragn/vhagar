#pragma once

#include "Actor/ActorBehavior.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderable/Mesh.hpp"

namespace vh
{

class MeshBehavior : public ActorBehavior
{
public:
    MeshBehavior(Actor* owner, const char* name);

    // -- ActorBehavior interface
    virtual void StartPlay() override;
    virtual void EndPlay() override;
    virtual void Tick(uint32_t delta) override;

    // -- indicates whether the mesh was loaded successfully
    bool IsValid();

    const Mesh* GetMesh() const { return mMesh.get(); }

private:
    GLBufferInfo mGLInfo;
    std::shared_ptr<Mesh> mMesh;
    glm::mat4 mModel;
    Renderer* mRenderer;

    GLuint mProgramID = 0;
    GLuint mProgram4ID = 0;
    GLuint mWireProgramID = 0;

    // uniform locations
    GLint uidMVP;
    GLint uidM;
    GLint uidV;
    GLint uidPLightPos;
    GLint uidPLightInt;
    GLint uidPLightNum;

    // wireframe uniform
    GLuint uidWireColor;
    GLuint uidWireMVP;
};

} // namespace vh
