#include "ActorFactory.hpp"

#include "CameraActor.hpp"
#include "FreeFloating.hpp"
#include "PointLightActor.hpp"
#include "StaticMeshActor.hpp"

#define REGISTER(name) Register(#name, &_Create<name>);

vh::ActorFactory::ActorFactory()
{
    REGISTER(CameraActor);
    REGISTER(FreeFloating);
    REGISTER(PointLightActor);
    REGISTER(StaticMeshActor);
}
