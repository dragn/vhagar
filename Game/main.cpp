#include "Vhagar.hpp"
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
            Renderer* render = GetComponent<Renderer>();
            World* world = GetComponent<World>();

            if (world && world->IsRunning() && render && render->IsRunning())
            {
                world->SpawnActor<StaticMeshActor>("Game/Assets/shadow_scene/shadow_scene.obj");
                world->SpawnActor<PointLight>(V3(0, 2, 4), 0.4);

                GetComponent<PlayerController>()->Control(world->SpawnActor<FreeFloating>());

                mSpawned = true;
            }
        }
    }

private:
    virtual void HandleEvent(SDL_Event *event)
    {
        vh::App::HandleEvent(event);
    }

    bool mSpawned;
};

int main(int argc, char ** argv) {

#ifdef WITH_GLOG
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
#endif

    MyApp app;

    app.AddComponent<Renderer>();
    app.AddComponent<World>();
    app.AddComponent<PlayerController>();

    app.Run();
}