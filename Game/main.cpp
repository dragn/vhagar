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
            Renderer* render = GetComponent<Renderer>();
            World* world = GetComponent<World>();

            if (world && world->IsRunning() && render && render->IsRunning())
            {
                world->SpawnActor<StaticMeshActor>("Assets/shadow_scene/shadow_scene.obj");
                world->SpawnActor<PointLight>(V3(0, 2, 4), 0.4);

                GetComponent<PlayerController>()->Control(world->SpawnActor<FreeFloating>());

                Overlay* overlay = new Overlay();
                render->AddObject(overlay);

                TTF_Font* font = TTF_OpenFont("Assets/Fonts/Roboto-Regular.ttf", 16);
                if (!font)
                {
                    LOG(ERROR) << "Could not open font";
                }
                else
                {
                    SDL_Surface* surf = TTF_RenderText_Solid(font, "Hello, World!", SDL_Color{ 255, 128, 128, 255 });
                    overlay->setBounds(Rect{ 0.1f, 0.1f, surf->w / 800.0f, surf->h / 450.0f});
                    overlay->setTexture(surf);

                    TTF_CloseFont(font);
                }

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

    RendererOptions ro;
    ro.screenWidth = 1600;
    ro.screenHeight = 900;

    app.AddComponent<Renderer>(ro);

    app.AddComponent<World>();
    app.AddComponent<PlayerController>();
    app.AddComponent<Console>();

    app.Run();

    return 0;
}
