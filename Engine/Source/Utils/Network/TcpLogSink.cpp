#include "Utils/VhUtils_PCH.hpp"
#include "TcpLogSink.hpp"

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
