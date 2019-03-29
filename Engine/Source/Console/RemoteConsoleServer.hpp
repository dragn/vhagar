#pragma once

#include "App/App.hpp"
#include "Network/TcpServer.hpp"
#include "Network/Network.hpp"
#include "ConsoleEngine.hpp"

namespace vh
{

class ClientLogSink : public google::LogSink
{
public:
    ClientLogSink(net::TcpClient* client);
    ~ClientLogSink();

    virtual void send(google::LogSeverity severity, const char* full_filename,
        const char* base_filename, int line,
        const struct ::tm* tm_time,
        const char* message, size_t message_len);

    // associated client
    net::TcpClient* mTcpClient;
};

class RemoteConsoleServer : public Component, public net::TcpServer::EventListener
{
    VH_COMPONENT(RemoteConsoleServer);

public:
    RemoteConsoleServer() : Component(TickFrequency::NORMAL)
    {
    }
    
    virtual Ret TickInit(uint32_t delta) override;
    virtual Ret TickRun(uint32_t delta) override;
    virtual Ret TickClose(uint32_t delta) override;

    /* TcpServer::EventListener interface */
    virtual void OnConnect(net::TcpClient*) override;
    virtual void OnDisconnect(net::TcpClient*) override;
    virtual void OnData(net::TcpClient* client, char* data, size_t dataSz) override;

private:
    std::list<std::unique_ptr<ClientLogSink>> mSinks;
    net::TcpServer mServer;
};

} // namespace vh
