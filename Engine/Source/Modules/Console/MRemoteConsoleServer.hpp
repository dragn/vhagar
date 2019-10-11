#pragma once

#include "Modules/Console/MConsoleEngine.hpp"
#include "Modules/Network/MNetwork.hpp"
#include "Modules/standard.hpp"

namespace vh
{


    /*
          //                     \\
        MMMM RemoteConsoleServer MMMM
          \\                     //

        Allows tcp access to the console engine.
    */
    class MRemoteConsoleServer : public Module, public net::TcpServer::EventListener
    {
        VH_MODULE(MRemoteConsoleServer);

    public:
        MRemoteConsoleServer() : Module(TickFrequency::NORMAL)
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
