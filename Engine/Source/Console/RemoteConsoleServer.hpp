#pragma once

#include "App/App.hpp"
#include "Network/TcpServer.hpp"
#include "Network/Network.hpp"
#include "Console.hpp"

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
    COMPONENT_NAME(RemoteConsoleServer);

public:
    RemoteConsoleServer() : Component(eTickFrequency::NORMAL)
    {
        App::CheckRequired<Console>();
        App::CheckRequired<Network>();
    }
    
    virtual void TickInit(uint32_t delta);
    virtual void TickRun(uint32_t delta);
    virtual void TickClose(uint32_t delta);

    /* TcpServer::EventListener interface */
    virtual void OnConnect(net::TcpClient*);
    virtual void OnDisconnect(net::TcpClient*);
    virtual void OnData(net::TcpClient* client, char* data, size_t dataSz);

private:
    std::list<ClientLogSink*> mSinks;
    net::TcpServer mServer;
};

} // namespace vh
