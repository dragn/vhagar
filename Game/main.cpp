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
            SDLRenderer* render = GetComponent<SDLRenderer>("Renderer");
            if (!render)
            {
                SpawnComponent<vh::SDLRenderer>("Renderer");
            }
            else if (render->IsRunning())
            {
                vh::Mesh* mesh = new vh::Mesh;
                vh::Utils::ImportWavefront(mesh, "Game/Assets/shadow_scene/shadow_scene.obj");

                render->AddObject(mesh);

                std::vector<vh::LightSource> lights;
                lights.push_back({ V3(0, 2, 4), 0.5 });
                lights.push_back({ V3(0, 3, -2), 0.3 });
                lights.push_back({ V3(0, 1, -5), 0.3 });

                render->SetLightSources(lights);

                M4 view = glm::lookAt(V3(0,2,5), V3(0, 0, 0), V3(0, 1, 0));
                render->SetView(view);

                LOG(INFO) << "Spawned!";
                mSpawned = true;
            }
        }
    }

private:
    bool mSpawned;
};

int main(int argc, char ** argv) {

#ifdef WITH_GLOG
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
#endif

    MyApp app;

    app.Run();
}
