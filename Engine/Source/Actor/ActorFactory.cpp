#include "ActorFactory.hpp"

#include "CameraActor.hpp"
#include "FreeFloating.hpp"
#include "PointLight.hpp"
#include "StaticMeshActor.hpp"

#define REGISTER(name) Register(#name, &_Create<name>);

vh::ActorFactory::ActorFactory()
{
    REGISTER(CameraActor);
    REGISTER(FreeFloating);
    REGISTER(PointLight);
    REGISTER(StaticMeshActor);
}
