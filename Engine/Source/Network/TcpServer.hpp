#pragma once

#include "net.hpp"

namespace net
{

class TcpServer
{
public:
    TcpServer(const InAddr& addr)
        : addr(addr)
    {}

private:
    InAddr addr;
};

} // namespace net
