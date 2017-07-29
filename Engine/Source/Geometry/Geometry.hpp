#pragma once

#include "App/App.hpp"

namespace vh
{

class Geometry : public Component
{
public:
    static const char* VH_COMPONENT;

    Geometry();

    virtual void TickInit(uint32_t delta);
    virtual void TickClose(uint32_t delta);
};

} // namespace vh
