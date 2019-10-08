#pragma once

#include "Utils/Logging.hpp"
#include "Socket.hpp"

#include <list>
#include "Poll.hpp"

static const size_t MAX_FDS = 16;
static const size_t BUFFER_SIZE = 512;

namespace net
{

class TcpClient
{
    friend class TcpServer;

public:
    TcpClient(int sock, const InAddr& addr)
        : mSocket(sock)
        , mAddr(addr)
    {}

    TcpClient(TcpClient&& other)
        : mSocket(std::move(other.mSocket))
        , mAddr(other.mAddr)
    {}

    const InAddr& GetAddr() const
    {
        return mAddr;
    }

    void SendString(const std::string& str)
    {
        SendData(str.c_str(), str.size());
    }

    void SendData(const char* data, size_t dataSize)
    {
        if (mSocket.GetState() == net::eClientStreamSocketState::Connected)
        {
            int rt = mSocket.Send(data, dataSize);
            if (rt < 0)
            {
                LOG(ERROR) << "Socket error: " << mSocket.GetError();
            }
        }
    }

private:
    ClientStreamSocket mSocket;
    InAddr mAddr;
    size_t mPollIdx;
};

class TcpServer
{
public:
    TcpServer()
        : mListener(nullptr)
    {
        ParseInAddr("127.0.0.1:20000", mAddr);
    }

    TcpServer(const InAddr& addr)
        : mAddr(addr)
        , mListener(nullptr)
    {}

    // open server socket
    void Init();

    // run polling cycle
    void Poll();

    // close all connections
    void Close();

    class EventListener
    {
    public:
        virtual void OnConnect(TcpClient*) {};
        virtual void OnDisconnect(TcpClient*) {};
        virtual void OnData(TcpClient* client, char* data, size_t dataSz) {};
    };

    void SetListener(EventListener* listener)
    {
        mListener = listener;
    }

    const InAddr& GetAddr() const
    {
        return mAddr;
    }

private:
    InAddr mAddr;
    EventListener* mListener;

    std::list<TcpClient> mClients;
    net::ServerStreamSocket mSocket;

    pollfd mFds[MAX_FDS];
    nfds_t mFdsSz;

    char mRecvBuf[BUFFER_SIZE];

    void _UpdateFds();
};

} // namespace net
