#include "Common.hpp"
#include "RemoteConsoleServer.hpp"

const char* vh::RemoteConsoleServer::COMPONENT_NAME = "RemoteConsoleServer";

void vh::RemoteConsoleServer::TickInit(uint32_t delta)
{
    mServer.SetListener(this);
    mServer.Init();

    LOG(INFO) << "Remote console server listening on " << mServer.GetAddr();

    FinishInit();
}

void vh::RemoteConsoleServer::TickRun(uint32_t delta)
{
    mServer.Poll();
}

void vh::RemoteConsoleServer::TickClose(uint32_t delta)
{
    mServer.Close();
    FinishClose();
}

void vh::RemoteConsoleServer::OnConnect(const net::TcpClient& clt)
{
    LOG(INFO) << "Remote console client connected: " << clt.GetAddr();
}

void vh::RemoteConsoleServer::OnDisconnect(const net::TcpClient& clt)
{
    LOG(INFO) << "Remote console client disconnected: " << clt.GetAddr();
}

void vh::RemoteConsoleServer::OnData(const net::TcpClient& clt, char* data, size_t dataSz)
{
    std::string str(data, dataSz);
    LOG(INFO) << "Command received from " << clt.GetAddr() << ": " << str;
}
