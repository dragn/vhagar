#pragma once

#include "App/App.hpp"

namespace vh
{

class Geometry : public Component
{
    VH_COMPONENT(Geometry);

public:
    Geometry();

    virtual Ret TickInit(uint32_t delta) override;
};

} // namespace vh
