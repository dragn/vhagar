#include "Common.hpp"

#include "PointLightActor.hpp"
#include "App/App.hpp"
#include "Renderer/Renderer.hpp"

namespace vh
{

void PointLightActor::OnCreate()
{
    App::Get<Renderer>()->AddLight(this);
}

void PointLightActor::OnDestroy()
{
    App::Get<Renderer>()->RemoveLight(this);
}

} // namespace vh
