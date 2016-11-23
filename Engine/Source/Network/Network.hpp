#pragma once

#include "App/Component.hpp"

namespace vh
{

/*
    non-ticking component responsible for network subsystems
*/
class Network : public Component
{
    COMPONENT_NAME(Network);

public:
    virtual void TickInit(uint32_t delta);
    virtual void TickClose(uint32_t delta);
};

} // namespace vh
