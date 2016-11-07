#include "Physics.hpp"

#include "App/App.hpp"

namespace vh
{

const char* Physics::COMPONENT_NAME = "PHYSICS";

Physics::Physics() : Component(COMPONENT_NAME) {}

void Physics::TickInit(uint32_t delta)
{
    FinishInit();
}

void Physics::TickRun(uint32_t delta)
{
}

void Physics::TickClose(uint32_t delta)
{
    FinishClose();
}

} // namespace vh
