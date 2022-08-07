#pragma once

#include "common/Log.hpp"

#ifdef __linux__
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

inline bool initNetworking() { return true; }
inline void closeConnection(int fd) { close(fd); }

#elif _WIN32
#include <winsock2.h>

inline bool initNetworking() {
  WSADATA wsaData;
  return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}
inline void closeConnection(int fd) { closesocket(fd); }

typedef int socklen_t;
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH

#else
#error "Networking is not supported for this platform!"
#endif
