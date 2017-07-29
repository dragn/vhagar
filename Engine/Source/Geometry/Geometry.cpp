#include "Common.hpp"

#include "Geometry/Geometry.hpp"
#include "App/World.hpp"
#include "Renderer/Renderer.hpp"
#include "Console/ConsoleCommands.hpp"

using namespace vh;

vh::Geometry::Geometry()
{
    App::CheckRequired<World>();
    App::CheckRequired<Renderer>();
}

/*
    spawn_box <pos> [<ambient color>] [<diffues color>] [<specular color>]
*/
DEFINE_COMMAND(spawn_box)
{
    if (params.size() < 4)
    {
        LOG(INFO) << "Usage: " << params[0] << " <pos> [<ambient color>] [<diffues color>] [<specular color>]";
        return;
    }

    if (params.size() >= 7)
    {
        // ambient color is set

    }

    /*
    Actor* box = App::Get<World>()->CreateActor("Box_");
    box->SetPos(V3(stof(params[1]), stof(params[2]), stof(params[3])));
    box->AddBehavior<MeshBehavior>("Assets/Meshes/box2.vhmesh");
    box->StartPlay();
    */
}

void vh::Geometry::TickInit(uint32_t delta)
{
    FinishInit();
}

void vh::Geometry::TickClose(uint32_t delta)
{
    FinishClose();
}
