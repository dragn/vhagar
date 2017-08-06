#include "Vhagar.hpp"

#include "SDL_ttf.h"
#include <glm/gtx/rotate_vector.hpp>

using namespace vh;
using namespace physx;

class FilterCallback : public PxQueryFilterCallback
{
public:
    virtual PxQueryHitType::Enum preFilter(const PxFilterData &filterData, const PxShape *shape, const PxRigidActor *actor, PxHitFlags &queryFlags)
    {
        if (actor)
        {
            LOG(INFO) << "prefilter " << actor;
        }
        return PxQueryHitType::eTOUCH;
    }
    virtual PxQueryHitType::Enum postFilter(const PxFilterData &filterData, const PxQueryHit &hit)
    {
        if (hit.actor)
        {
            LOG(INFO) << "postfilter " << hit.actor;
        }
        return PxQueryHitType::eTOUCH;
    }
};

class CustomPlayerController : public PlayerController
{
public:
    virtual void HandleEvent(SDL_Event *event) override
    {
        PlayerController::HandleEvent(event);

        switch (event->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button = SDL_BUTTON_LEFT)
            {
                CameraBehavior* cam = GetControlledActor()->GetBehaviorOfType<CameraBehavior>();
                if (mHolding)
                {
                    PxRigidDynamic* actor = mHolding->GetBehaviorOfType<PhysicsBehavior>()->GetRigidActor()->is<PxRigidDynamic>();
                    actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);
                    actor->addForce(ToPhysX(cam->GetForward() * 50.0f), PxForceMode::eIMPULSE);
                    mHolding = nullptr;
                }
                else
                {
                    PxScene* scene = App::Get<Physics>()->GetScene();
                    PxVec3 origin = ToPhysX(cam->GetPos());
                    PxVec3 unitDir = ToPhysX(cam->GetForward()).getNormalized();
                    PxReal maxDistance = 100.0f;
                    PxRaycastBuffer hits;
                    PxRaycastHit buf[5];
                    hits.touches = buf;
                    hits.maxNbTouches = 5;
                    PxQueryFilterData filterData(PxQueryFlag::eDYNAMIC | PxQueryFlag::eNO_BLOCK);
                    FilterCallback callback;
                    bool status = scene->raycast(origin, unitDir, maxDistance, hits, PxHitFlag::eDEFAULT, filterData, &callback);
                    if (status)
                    {
                        LOG(INFO) << "Trace " << hits.nbTouches << " hits";
                        for (uint32_t i = 0; i < hits.nbTouches; i++)
                        {
                            PxRaycastHit& hit = hits.touches[i];
                            if (hit.actor->is<PxRigidDynamic>() && hit.actor->userData != nullptr)
                            {
                                mHolding = reinterpret_cast<Actor*>(hit.actor->userData);
                                hit.actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
                                LOG(INFO) << "Catch " << mHolding->GetName();
                            }
                        }
                    }
                }
            }
        }
    }
    virtual void TickRun(uint32_t delta) override
    {
        PlayerController::TickRun(delta);

        Actor* actor = GetControlledActor();
        if (actor && mHolding)
        {
            PhysicsBehavior* pb = mHolding->GetBehaviorOfType<PhysicsBehavior>();
            if (pb)
            {
                PxRigidDynamic* dynamic = pb->GetRigidActor()->is<PxRigidDynamic>();
                if (dynamic)
                {
                    PxVec3 holdPos = ToPhysX(actor->GetPos() + V3(0, 0.25, 0) + actor->GetBehaviorOfType<CameraBehavior>()->GetForward());
                    PxVec3 vel = dynamic->getLinearVelocity();
                    PxVec3 pos = dynamic->getGlobalPose().p;
                    float dt = 1.0f / 60.0f;

                    float freq = 10.0f;
                    float kp = 36.0f * freq * freq * 0.25f;
                    float kd = 4.5f * freq;
                    float g = 1 / (1 + kd * dt + kp * dt * dt);
                    float ksg = kp * g;
                    float kdg = (kd + kp * dt) * g;
                    PxVec3 F = (holdPos - pos) * ksg + ( - vel) * kdg;

                    dynamic->addForce(F);
                }
            }
        }
    }

private:
    Actor* mHolding = nullptr;
};

class CustomCharacterBehavior : public CapsuleCharacterBehavior
{
public:
    CustomCharacterBehavior(float radius, float height)
        : CapsuleCharacterBehavior(radius, height)
    {
    }

    virtual void onShapeHit(const physx::PxControllerShapeHit& hit) override
    {
        PxRigidBody* body = hit.actor->is<PxRigidBody>();
        if (body)
        {
            PxVec3 vec = hit.actor->getGlobalPose().p - ToPhysX(mOwner->GetPos());
            body->addForce(ToPhysX(mOwner->GetForward()) * 100.0f);
        }
    }
};

void AddPointLight(World* world, V3 pos, float intensity)
{
    Actor* actor = world->CreateActor("Light");
    actor->AddBehavior<PointLightBehavior>(intensity);
    actor->SetPos(pos);
    actor->StartPlay();
}

void SpawnBox(World* world)
{
    Actor* box = world->CreateActor("Box");
    box->AddPos(V3((float) 2 * rand() / (float) RAND_MAX - 1.0f, 0.0f, (float) 2 * rand() / (float) RAND_MAX - 1.0f));
    box->SetScale(V3(0.2, 0.2, 0.2));
    box->AddBehavior<MeshBehavior>("Assets/Meshes/box2.vhmesh");
    MeshBehavior* mb = box->AddBehavior<MeshBehavior>("Assets/Meshes/box2.vhmesh");
    mb->SetRelPos(V3(0.15f, 0.15f, 0.15f));
    mb->SetRelScale(V3(0.5f, 0.5f, 0.5f));
    mb->SetRelRot(glm::quat(V3(0.7f, 0.0f, 0.0f)));
    PhysicsBehavior* pb = box->AddBehavior<PhysicsBehavior>(false);
    pb->SetBoxGeometry(V3(0.2f, 0.2f, 0.2f));
    box->SetPitch(0.2);
    box->StartPlay();
}

void StaticBox(V3 pos, V3 size)
{
    World* world = App::Get<World>();
    Actor* mesh = world->CreateActor("Static");
    mesh->AddPos(pos);
    mesh->SetScale(size);
    mesh->AddBehavior<MeshBehavior>("Assets/Meshes/box2.vhmesh");
    PhysicsBehavior* pb = mesh->AddBehavior<PhysicsBehavior>();
    pb->SetBoxGeometry(size);
    mesh->StartPlay();
}

class MyApp : public vh::App
{
public:
    MyApp() : mSpawned(false) {}

    void OnTick()
    {
        if (!mSpawned)
        {
            Renderer* render = Get<Renderer>();
            World* world = Get<World>();

            if (world && world->IsRunning() && render && render->IsRunning())
            {
                AddPointLight(world, V3(0, 0, 0), 0.4f);
                // AddPointLight(world, V3(5, 0, 0), 0.4f);
                /*
                StaticMeshActor* actor = world->SpawnActor<StaticMeshActor>("Assets/box2.obj");
                actor->SetScale(V3(3.0f, 0.2f, 3.0f));
                actor->SetPos(V3(0.0f, -1.0f, 0.0f));
                */

                /*
                Mesh* mesh = new Mesh();
                mesh->SetDim(4);
                GLfloat plane[] = { // 5 vertices, pos in homogeneous, normal + 3 colors
                    0, 0, 0, 1,     1, 0, 0, 0,     0, 0, -1, 0,   -1, 0, 0, 0,     0, 0, 1, 0,     // pos
                    0, 1, 0,        0, 1, 0,        0, 1, 0,        0, 1, 0,        0, 1, 0,        // normal
                    0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  // colors
                    0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  // colors
                    0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  0.5, 0.5, 0.5,  // colors
                };
                GLfloat* pPlane = new GLfloat[5 * 16];
                std::copy(plane, plane + 5 * 16, pPlane);

                mesh->SetAttribData(5, 4, pPlane);
                GLuint index[] = {
                    0, 1, 2,
                    0, 2, 3,
                    0, 3, 4,
                    0, 4, 1
                };
                GLuint* pIndex = new GLuint[3 * 4];
                std::copy(index, index + 3 * 4, pIndex);
                mesh->SetIndexData(4 * 3, pIndex);

                StaticMeshActor* planeActor = world->SpawnActor<StaticMeshActor>();
                planeActor->SetMesh(mesh);
                */

                // make a room
                StaticBox(V3(0, -2, 0), V3(4, 0.2, 4)); // floor
                StaticBox(V3(0, 0, -4), V3(4, 2, 0.2));
                StaticBox(V3(0, 0, 4), V3(4, 2, 0.2));
                StaticBox(V3(4, 0, 0), V3(0.2, 2, 4));
                StaticBox(V3(-4, 0, 0), V3(0.2, 2, 4));

                Utils::CubeMap cube;
                cube.pos_x = "Assets/images/space/pink_planet_pos_x.tga";
                cube.neg_x = "Assets/images/space/pink_planet_neg_x.tga";
                cube.pos_y = "Assets/images/space/pink_planet_pos_y.tga";
                cube.neg_y = "Assets/images/space/pink_planet_neg_y.tga";
                cube.pos_z = "Assets/images/space/pink_planet_pos_z.tga";
                cube.neg_z = "Assets/images/space/pink_planet_neg_z.tga";

                Actor* skyBox = world->CreateActor("SkyBox");
                skyBox->AddBehavior<SkyBoxBehavior>(cube);
                skyBox->StartPlay();

                Actor* character = world->CreateActor("Character");
                character->AddBehavior<CustomCharacterBehavior>(0.4f, 0.5f);

                CameraBehavior* cb = character->AddBehavior<CameraBehavior>();
                cb->SetRelPos(V3(0.0f, 0.25f, 0.0f));

                MeshBehavior* mb = cb->AddChild<MeshBehavior>("Assets/Meshes/box2.vhmesh");
                mb->SetRelPos(V3(0.5f, -0.5f, -0.7f));
                mb->SetRelScale(V3(0.1f, 0.1f, 0.6f));
                character->StartPlay();

                Get<PlayerController>()->Control(character);

                mSpawned = true;
            }
        }
    }

protected:
    virtual void HandleEvent(SDL_Event *event) override
    {
        vh::App::HandleEvent(event);
        IF_KEYDOWN_SYM(event, SDLK_RETURN)
        {
            SpawnBox(Get<World>());
        }
    }

private:
    bool mSpawned;
};

int main(int argc, char ** argv)
{
    MyApp app;

    srand(clock());

    RendererOptions ro;
    ro.screenWidth = 1600;
    ro.screenHeight = 900;
    ro.antialias = RendererOptions::AA_4X;
    ro.monitor = RendererOptions::MON_SECOND;
    ro.borderless = false;

    app.AddComponent<Renderer>(ro);
    app.AddComponent<ConsoleEngine>();
    app.AddComponent<Console>();
    app.AddComponent<ResourceSystem>();
    app.AddComponent<Physics>();
    app.AddComponent<World>();
    app.AddComponent<CustomPlayerController>();
    app.AddComponent<Debug>();

    app.Run();

    return 0;
}
