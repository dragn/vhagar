#include "Modules/VhModules_PCH.hpp"
#include "MNetwork.hpp"

VH_MODULE_IMPL(vh::MNetwork);

vh::Ret vh::MNetwork::TickInit(uint32_t delta)
{
    return net::Init() ? Ret::SUCCESS : Ret::FAILURE;
}

vh::Ret vh::MNetwork::TickClose(uint32_t delta)
{
    net::Close();
    return Ret::SUCCESS;
}
