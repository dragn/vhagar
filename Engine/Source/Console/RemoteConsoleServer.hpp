#pragma once

#include "App/App.hpp"
#include "Network/TcpServer.hpp"
#include "Network/Network.hpp"

namespace vh
{

class RemoteConsoleServer : public Component, public net::TcpServer::EventListener
{
public:
    static const char* COMPONENT_NAME;

    RemoteConsoleServer() : Component(COMPONENT_NAME, 16)
    {
        App::CheckRequired<Network>();
    }
    
    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    /* TcpServer::EventListener interface */
    virtual void OnConnect(const net::TcpClient&);
    virtual void OnDisconnect(const net::TcpClient&);
    virtual void OnData(const net::TcpClient& client, char* data, size_t dataSz);

private:
    net::TcpServer mServer;
};

} // namespace vh
