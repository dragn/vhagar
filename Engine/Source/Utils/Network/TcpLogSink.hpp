#pragma once

#include "Utils/Logging.hpp"
#include "Utils/Network/TcpServer.hpp"
#include "Utils/standard.hpp"

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

} // namespace vh
