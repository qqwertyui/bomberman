#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include "ConfigLoader.hpp"
#include "common/Log.hpp"
#include "messages/serverInfo.pb.h"

struct ClientInfo {
  int fd;
  std::string ip;
  unsigned int port;
};

void handleClient(const ClientInfo clientInfo) {
  LOG_INF("[+] Connected from %s:%u", clientInfo.ip.c_str(), clientInfo.port);

  bomberman::ServerInfo si;
  si.set_version("0.1");
  bomberman::ServerInfo::Lobby *lobby1 = si.add_lobbies();
  lobby1->set_id(1);
  lobby1->set_players(0);
  bomberman::ServerInfo::Lobby *lobby2 = si.add_lobbies();
  lobby2->set_id(2);
  lobby2->set_players(0);

  std::string message;
  si.SerializeToString(&message);
  send(clientInfo.fd, (const void *)message.c_str(), message.size(), 0);

  close(clientInfo.fd);
  LOG_INF("[-] Disconnected from %s:%u", clientInfo.ip.c_str(),
          clientInfo.port);
}

int main(int argc, char **argv) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  bomberman::ConfigLoader configLoader{argc, argv};
  const auto &config{configLoader.get()};
  int serverFd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverFd == -1) {
    LOG_ERR("Socket creation failed: %s", strerror(errno));
    return 1;
  }
  struct sockaddr_in server_sin;
  server_sin.sin_family = AF_INET;
  server_sin.sin_port = htons(config.port);
  server_sin.sin_addr.s_addr = inet_addr(config.bindIp.c_str());

  int status = bind(serverFd, (const struct sockaddr *)&server_sin,
                    sizeof(struct sockaddr_in));
  if (status == -1) {
    LOG_ERR("Bind failed: %s", strerror(errno));
    return 1;
  }
  status = listen(serverFd, config.backlogSize);
  if (status == -1) {
    LOG_ERR("Listen failed: %s", strerror(errno));
    return 1;
  }
  LOG_INF("Listening for incoming connections on %s:%u", config.bindIp.c_str(),
          config.port);
  while (1) {
    struct sockaddr_in client_sin;
    socklen_t socklen = sizeof(client_sin);
    int clientFd = accept(serverFd, (struct sockaddr *)&client_sin, &socklen);
    if (clientFd == -1) {
      LOG_ERR("Accept failed: %s", strerror(errno));
      continue;
    }
    ClientInfo clientInfo{clientFd, inet_ntoa(client_sin.sin_addr),
                          htons(client_sin.sin_port)};
    std::thread clientThread(handleClient, clientInfo);
    clientThread.detach();
  }
  return 0;
}