#include "Modules/VhModules_PCH.hpp"
#include "MRemoteConsoleServer.hpp"

VH_MODULE_IMPL(vh::MRemoteConsoleServer);

vh::Ret vh::MRemoteConsoleServer::TickInit(uint32_t delta)
{
    App::CheckRequired<MConsoleEngine>();
    App::CheckRequired<MNetwork>();

    mServer.SetListener(this);
    mServer.Init();

    LOG(INFO) << "Remote console server listening on " << mServer.GetAddr();

    return Ret::SUCCESS;
}

vh::Ret vh::MRemoteConsoleServer::TickRun(uint32_t delta)
{
    mServer.Poll();

    return Ret::CONTINUE;
}

vh::Ret vh::MRemoteConsoleServer::TickClose(uint32_t delta)
{
    mSinks.clear();
    mServer.Close();

    return Ret::SUCCESS;
}

void vh::MRemoteConsoleServer::OnConnect(net::TcpClient* clt)
{
    LOG(INFO) << "Remote console client connected: " << clt->GetAddr();

    mSinks.push_back(std::make_unique<ClientLogSink>(clt));
}

void vh::MRemoteConsoleServer::OnDisconnect(net::TcpClient* clt)
{
    LOG(INFO) << "Remote console client disconnected: " << clt->GetAddr();

    std::remove_if(mSinks.begin(), mSinks.end(), [clt] (const std::unique_ptr<ClientLogSink>& sink)
    {
        return sink->mTcpClient == clt;
    });
}

void vh::MRemoteConsoleServer::OnData(net::TcpClient* clt, char* data, size_t dataSz)
{
    std::string str(data, dataSz);
    LOG(INFO) << "Command received from " << clt->GetAddr() << ": " << str;
    MConsoleEngine* console = App::Get<MConsoleEngine>();
    console->Exec(str);
}
