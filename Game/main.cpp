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
                world->SpawnActor<StaticMeshActor>("Assets/shadow_scene/shadow_scene.obj");
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

        if (event->type == SDL_KEYUP)
        {
            SDL_Keycode k = event->key.keysym.sym;
            if (k == 'p')
            {
                GetComponent<Console>()->Exec("print hello");
            }
        }
    }

    bool mSpawned;
};

int main(int argc, char ** argv) {
    MyApp app;

    app.AddComponent<Renderer>();
    app.AddComponent<World>();
    app.AddComponent<PlayerController>();
    app.AddComponent<Console>();

    app.Run();

    return 0;
}
