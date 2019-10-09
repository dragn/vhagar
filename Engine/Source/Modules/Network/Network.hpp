#pragma once

#include "Modules/standard.hpp"

namespace vh
{

/*
    non-ticking component responsible for network subsystems
*/
class Network : public Module
{
    VH_MODULE(Network);

public:
    virtual Ret TickInit(uint32_t delta) override;
    virtual Ret TickClose(uint32_t delta) override;
};

} // namespace vh
