#include "Common.hpp"

#include "net.hpp"
#include "Network.hpp"

const char* vh::Network::COMPONENT_NAME = "Network";

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
