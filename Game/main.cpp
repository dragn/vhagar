#include "Vhagar.hpp"

#include "SDL_ttf.h"
#include <glm/gtx/transform.hpp>

using namespace vh;

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
    box->AddBehavior<MeshBehavior>("Assets/Sources/box2.obj");
    PhysicsBehavior* pb = box->AddBehavior<PhysicsBehavior>(false);
    pb->SetBoxGeometry(V3(0.2f, 0.2f, 0.2f));
    box->SetPitch(0.2);
    box->StartPlay();
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
                AddPointLight(world, V3(0, 2, 4), 0.4f);
                AddPointLight(world, V3(5, 0, 0), 0.4f);
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

                Actor* mesh = world->CreateActor("Plane");
                mesh->AddPos(V3(0, -2, 0));
                mesh->SetScale(V3(5.0f, 0.2f, 5.0f));
                mesh->AddBehavior<MeshBehavior>("Assets/Sources/box2.obj");
                PhysicsBehavior* pb = mesh->AddBehavior<PhysicsBehavior>();
                pb->SetBoxGeometry(V3(5.0f, 0.2f, 5.0f));
                mesh->StartPlay();


                Actor* ff = world->CreateActor("FreeFloating");
                ff->AddPos(V3(0, 0, 2));
                ff->AddBehavior<FreeFloatingBehavior>();
                ff->StartPlay();

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

                Get<PlayerController>()->Control(ff);

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

    app.AddComponent<Renderer>(ro);
    app.AddComponent<ConsoleEngine>();
    app.AddComponent<Console>();
    app.AddComponent<ResourceSystem>();
    app.AddComponent<Physics>();
    app.AddComponent<World>();
    app.AddComponent<PlayerController>();
    //app.AddComponent<Debug>();

    app.Run();

    return 0;
}
