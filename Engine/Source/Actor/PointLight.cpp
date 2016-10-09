#include "PointLight.hpp"

#include "App/App.hpp"
#include "Renderer/Renderer.hpp"

namespace vh
{

void PointLight::OnCreate()
{
    Renderer* render = App::GetComponent<Renderer>();
    if (render)
    {
        Light light;
        light.position = GetPos();
        light.intensity = mIntensity;
        LOG(INFO) << "Add light " << light.position << " " << light.intensity;
        mLightId = render->AddLight(light);
    }
}

void PointLight::OnDestroy()
{
    Renderer* render = App::GetComponent<Renderer>();
    if (render)
    {
        render->RemoveLight(mLightId);
    }
}

} // namespace vh
