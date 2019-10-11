#include "Utils/VhUtils_PCH.hpp"
#include "TcpServer.hpp"

void net::TcpServer::Init()
{
    if (!mSocket.Listen(mAddr))
    {
        LOG(ERROR) << "Socket error: " << mSocket.GetError() << std::endl;
    }
    mFdsSz = 1;
    mFds[0].fd = mSocket.Get();
    mFds[0].events = POLLIN;
    mFds[0].revents = 0;
}

void net::TcpServer::Poll()
{
    while (poll(mFds, mFdsSz, 0) > 0)
    {
        bool needUpdate = false;

        // check for incoming connections
        if (mFds[0].revents & POLLIN)
        {
            int cltFd;
            net::InAddr cltAddr;
            while (mSocket.Accept(cltFd, cltAddr))
            {
                if (mFdsSz == MAX_FDS)
                {
                    LOG(WARNING) << "Connection limit exceeded";
                    net::ClientStreamSocket css(cltFd);
                    css.Close();
                }
                else
                {
                    mClients.push_back(TcpClient(cltFd, cltAddr));
                    if (mListener) mListener->OnConnect(&mClients.back());
                    needUpdate = true;
                }
            }
        }

        if (needUpdate)
        {
            _UpdateFds();
            needUpdate = false;
        }

        for (auto client = mClients.begin(); client != mClients.end();)
        {
            net::ClientStreamSocket& mSocket = client->mSocket;
            const pollfd& pfd = mFds[client->mPollIdx];
            if (pfd.revents & POLLIN)
            {
                int sz = mSocket.Recv(mRecvBuf, sizeof(mRecvBuf));
                if (sz < 0)
                {
                    LOG(ERROR) << "Socket error: " << mSocket.GetError();
                }
                else if (sz > 0)
                {
                    if (mListener) mListener->OnData(&*client, mRecvBuf, sz);
                }
            }
            if (pfd.revents & (POLLHUP | POLLERR))
            {
                mSocket.Close();
            }
            if (mSocket.IsClosed())
            {
                if (mListener) mListener->OnDisconnect(&*client);
                client = mClients.erase(client);
                needUpdate = true;
            }
            else
            {
                client++;
            }
        }

        if (needUpdate) _UpdateFds();
    }
}

void net::TcpServer::Close()
{
    for (TcpClient& client : mClients)
    {
        client.mSocket.Close();
    }
    mSocket.Close();
}

void net::TcpServer::_UpdateFds()
{
    auto client = mClients.begin();
    mFdsSz = 1;
    while (client != mClients.end())
    {
        mFds[mFdsSz].fd = client->mSocket.Get();
        mFds[mFdsSz].events = POLLIN;
        mFds[mFdsSz].revents = 0;

        client->mPollIdx = mFdsSz;

        mFdsSz++;
        client++;
    }
}
