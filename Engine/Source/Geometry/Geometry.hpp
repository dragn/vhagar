#pragma once

#include "App/App.hpp"

namespace vh
{

class Geometry : public Component
{
public:
    static const char* COMPONENT_NAME;

    Geometry();

    virtual void TickInit(uint32_t delta);
    virtual void TickClose(uint32_t delta);
};

} // namespace vh
