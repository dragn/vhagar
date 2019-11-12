#include "Modules/VhModules_PCH.hpp"
#include "BMesh_GL.hpp"

#include "Modules/ResourceSystem/MResourceSystem.hpp"

vh::BMesh_GL::BMesh_GL()
{
}

vh::BMesh_GL::~BMesh_GL()
{
}

void vh::BMesh_GL::StartPlay()
{
    mMesh = App::Get<MResourceSystem>()->GetResource<RMesh_GL>(GetPath().c_str());
}

void vh::BMesh_GL::EndPlay()
{

}

void vh::BMesh_GL::Tick(uint32_t delta)
{

}
