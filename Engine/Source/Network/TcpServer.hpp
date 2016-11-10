#pragma once

#include "net.hpp"

#include <list>

#ifdef CMAKE_PLATFORM_WINDOWS
#include <WinSock2.h>
#define poll WSAPoll
#define pollfd WSAPOLLFD
typedef ULONG nfds_t;
#endif // CMAKE_PLATFORM_WINDOWS

#ifdef CMAKE_PLATFORM_UNIX
#include <poll.h>
#endif // CMAKE_PLATFORM_UNIX

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
        virtual void OnConnect(const TcpClient&) {};
        virtual void OnDisconnect(const TcpClient&) {};
        virtual void OnData(const TcpClient& client, char* data, size_t dataSz) {};
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
