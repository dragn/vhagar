#include "Modules/VhModules_PCH.hpp"
#include "Network.hpp"

#include "Modules/Network/Socket.hpp"

VH_MODULE_IMPL(vh::Network);

vh::Ret vh::Network::TickInit(uint32_t delta)
{
    return net::Init() ? Ret::SUCCESS : Ret::FAILURE;
}

vh::Ret vh::Network::TickClose(uint32_t delta)
{
    net::Close();
    return Ret::SUCCESS;
}
