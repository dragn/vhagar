#include "Common.hpp"

#include "PointLight.hpp"
#include "App/App.hpp"
#include "Renderer/Renderer.hpp"

namespace vh
{

void PointLight::OnCreate()
{
    App::Get<Renderer>()->AddLight(this);
}

void PointLight::OnDestroy()
{
    App::Get<Renderer>()->RemoveLight(this);
}

} // namespace vh
