#pragma once

#include "App/Component.hpp"

namespace vh
{

/*
    non-ticking component responsible for network subsystems
*/
class Network : public Component
{
public:
    static const char* COMPONENT_NAME;

    Network()
        : Component(COMPONENT_NAME, -1)
    {}

    virtual void TickInit(uint32_t delta);
    virtual void TickClose(uint32_t delta);
};

} // namespace vh
