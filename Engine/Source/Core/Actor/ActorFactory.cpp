#include "Core/VhCore_PCH.hpp"

#include "ActorFactory.hpp"

#define REGISTER(name) Register(#name, &_Create<name>);

vh::ActorFactory::ActorFactory()
{
}
