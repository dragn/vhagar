#pragma once

#ifdef CMAKE_PLATFORM_WINDOWS
#include <WinSock2.h>
#define poll WSAPoll
#define pollfd WSAPOLLFD
typedef ULONG nfds_t;
#endif // CMAKE_PLATFORM_WINDOWS

#ifdef CMAKE_PLATFORM_UNIX
#include <poll.h>
#endif // CMAKE_PLATFORM_UNI
