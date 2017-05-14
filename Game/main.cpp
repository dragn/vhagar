#include "Vhagar.hpp"

#include "SDL_ttf.h"
#include <glm/gtx/transform.hpp>

using namespace vh;

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
                world->SpawnActor<PointLightActor>(V3(0, 2, 4), 0.4f);
                world->SpawnActor<PointLightActor>(V3(5, 0, 0), 0.4f);
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

                Get<PlayerController>()->Control(world->SpawnActor<FreeFloating>());

                mSpawned = true;
            }
        }
    }

private:
    bool mSpawned;
};

int main(int argc, char ** argv) {
    MyApp app;

    RendererOptions ro;
    ro.screenWidth = 1600;
    ro.screenHeight = 900;
    ro.antialias = RendererOptions::AA_4X;
    ro.monitor = RendererOptions::MON_SECOND;

    app.AddComponent<Renderer>(ro);
    app.AddComponent<World>();
    app.AddComponent<ConsoleEngine>();
    app.AddComponent<Console>();
    app.AddComponent<PlayerController>();
    app.AddComponent<ResourceSystem>();
    app.AddComponent<Debug>();
    app.AddComponent<Physics>();

    app.Run();

    return 0;
}
