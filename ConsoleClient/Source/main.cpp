#include "Network/Socket.hpp"
#include "Network/Poll.hpp"

#include "Utils/Thread.hpp"

#include <iostream>
#include <string>

const char* PROMPT = "> ";

static int SocketRead(void* param)
{
    net::ClientStreamSocket* sock = reinterpret_cast<net::ClientStreamSocket*>(param);

    CHECK(sock);

    char buf[256];

    pollfd fds[1];

    fds[0].fd = sock->Get();
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    while (true) if (poll(fds, 1, -1) > 0)
    {
        if (sock->IsClosed()) break;

        if (fds[0].revents & POLLIN)
        {
            std::cout << "\b\b";
            int bytes = sock->Recv(buf, 255);
            if (bytes < 0)
            {
                std::cout << "Connection error: " << sock->GetError() << std::endl;
            }
            else if (bytes > 0)
            {
                buf[bytes] = '\0';
                std::cout << buf << std::endl;
                std::cout << PROMPT;
            }
        }
        if (fds[0].revents & (POLLHUP | POLLERR))
        {
            sock->Close();
            break;
        }
    }

    return 0;
}

int main(int argc, char** argv)
{
    net::ScopeInit init;

    const char* addrStr = "127.0.0.1:20000";

    if (argc > 1) addrStr = argv[1];

    net::InAddr addr;
    net::ParseInAddr(addrStr, addr);

    net::ClientStreamSocket sock;

    if (!sock.Connect(addr))
    {
        std::cerr << "Socket error: " << sock.GetError() << std::endl;
        return -1;
    }

    std::cout << PROMPT;

    vh::Thread sockRead;
    sockRead.Start(SocketRead, &sock);

    while (!sock.IsClosed())
    {
        std::string cmd;
        std::getline(std::cin, cmd);

        if (cmd == "quit")
        {
            sock.Close();
            break;
        }

        std::cout << PROMPT;
        if (sock.Send(cmd.c_str(), cmd.size()) < 0)
        {
            std::cout << "Socket error: " << sock.GetError() << std::endl;
        }
    }

    sockRead.Wait(TIME_INFINITE);

    return 0;
}
