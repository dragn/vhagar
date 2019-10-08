#include "Vhagar_PCH.hpp"

#include "Socket.hpp"
#include "Network.hpp"

VH_COMPONENT_IMPL(vh::Network);

vh::Ret vh::Network::TickInit(uint32_t delta)
{
    return net::Init() ? Ret::SUCCESS : Ret::FAILURE;
}

vh::Ret vh::Network::TickClose(uint32_t delta)
{
    net::Close();
    return Ret::SUCCESS;
}
