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
                world->SpawnActor<PointLightActor>(V3(0, 2, 4), 0.4);

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
    app.AddComponent<Console>();
    app.AddComponent<PlayerController>();
    app.AddComponent<ResourceSystem>();

    app.Run();

    return 0;
}
