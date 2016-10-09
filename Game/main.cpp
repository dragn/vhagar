#include "Vhagar.hpp"
#include <glm/gtx/transform.hpp>

using namespace vh;

class MyApp : public vh::App
{
public:
    MyApp() : mSpawned(false), mLight(nullptr) {}

    void OnTick()
    {
        if (!mSpawned)
        {
            Renderer* render = GetComponent<Renderer>();
            World* world = GetComponent<World>();

            if (world && world->IsRunning() && render && render->IsRunning())
            {
                mMesh = world->SpawnActor<StaticMeshActor>("Game/Assets/shadow_scene/shadow_scene.obj");

                M4 view = glm::lookAt(V3(0,2,5), V3(0, 0, 0), V3(0, 1, 0));
                render->SetView(view);

                LOG(INFO) << "Spawned!";
                mSpawned = true;
            }
        }
    }

private:
    virtual void HandleEvent(SDL_Event *event)
    {
        vh::App::HandleEvent(event);

        if (event->type == SDL_KEYDOWN)
        {
            if (event->key.keysym.sym == '1')
            {
                World* world = GetComponent<World>();
                if (mLight == nullptr)
                {
                    mLight = world->SpawnActor<PointLight>(V3(0, 2, 4), 0.4);
                }
                else
                {
                    world->DestroyActor(mLight);
                }
            }
            if (event->key.keysym.sym == 'a')
            {
                mMesh->AddPos(V3(0, 1, 0));
            }
            if (event->key.keysym.sym == 'd')
            {
                mMesh->AddPos(V3(0, -1, 0));
            }
        }
    }

    bool mSpawned;
    PointLight* mLight;
    Actor* mMesh;
};

int main(int argc, char ** argv) {

#ifdef WITH_GLOG
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
#endif

    MyApp app;

    app.AddComponent<Renderer>();
    app.AddComponent<World>();

    app.Run();
}
