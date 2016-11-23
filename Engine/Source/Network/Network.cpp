#include "Common.hpp"

#include "Socket.hpp"
#include "Network.hpp"

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
