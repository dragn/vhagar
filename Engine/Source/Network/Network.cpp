#include "Vhagar_PCH.hpp"

#include "Socket.hpp"
#include "Network.hpp"

VH_COMPONENT_IMPL(vh::Network);

void vh::Network::TickInit(uint32_t delta)
{
    net::Init();
    FinishInit();
}

void vh::Network::TickClose(uint32_t delta)
{
    net::Close();
    FinishClose();
}
