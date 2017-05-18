#include "Common.hpp"
#include "RemoteConsoleServer.hpp"

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
    mSinks.clear();
    mServer.Close();
    FinishClose();
}

void vh::RemoteConsoleServer::OnConnect(net::TcpClient* clt)
{
    LOG(INFO) << "Remote console client connected: " << clt->GetAddr();

    mSinks.push_back(std::make_unique<ClientLogSink>(clt));
}

void vh::RemoteConsoleServer::OnDisconnect(net::TcpClient* clt)
{
    LOG(INFO) << "Remote console client disconnected: " << clt->GetAddr();

    std::remove_if(mSinks.begin(), mSinks.end(), [clt] (const std::unique_ptr<ClientLogSink>& sink)
    {
        return sink->mTcpClient == clt;
    });
}

void vh::RemoteConsoleServer::OnData(net::TcpClient* clt, char* data, size_t dataSz)
{
    std::string str(data, dataSz);
    LOG(INFO) << "Command received from " << clt->GetAddr() << ": " << str;
    ConsoleEngine* console = App::Get<ConsoleEngine>();
    console->Exec(str);
}

vh::ClientLogSink::ClientLogSink(net::TcpClient* client) : mTcpClient(client)
{
    LOG(INFO) << "Client log sink created for " << mTcpClient->GetAddr();
    google::AddLogSink(this);
}

vh::ClientLogSink::~ClientLogSink()
{
    google::RemoveLogSink(this);
    LOG(INFO) << "Client log sink destroyed for " << mTcpClient->GetAddr();
}

void vh::ClientLogSink::send(google::LogSeverity severity, const char* full_filename, const char* base_filename, int line, const struct ::tm* tm_time, const char* message, size_t message_len)
{
    CHECK(mTcpClient);
    mTcpClient->SendString(ToString(severity, base_filename, line, tm_time, message, message_len));
    mTcpClient->SendData("\n", 1);
}
