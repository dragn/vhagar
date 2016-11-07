#pragma once

#include "App/Component.hpp"

namespace vh
{

class Physics : public Component
{
public:
    static const char* COMPONENT_NAME;

    Physics();

    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);
};

} // namespace vh
