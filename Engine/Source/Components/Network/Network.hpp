#pragma once

#include "App/Component.hpp"

namespace vh
{

/*
    non-ticking component responsible for network subsystems
*/
class Network : public Component
{
    VH_COMPONENT(Network);

public:
    virtual Ret TickInit(uint32_t delta) override;
    virtual Ret TickClose(uint32_t delta) override;
};

} // namespace vh
