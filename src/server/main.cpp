#include "GlobalConfig.hpp"
#include "common/ConnectionManager.hpp"
#include "common/Log.hpp"
#include "common/Networking.hpp"
#include "common/messages/core.pb.h"

#include <thread>
#include <unistd.h>

struct ClientInfo {
  int fd;
  std::string ip;
  unsigned int port;
};

void handleQuery(const bomberman::QueryReq &req, bomberman::QueryResp &resp) {
  if (req.has_version()) {
    resp.set_version(bomberman::GlobalConfig::get().version());
  }
  if (req.has_lobbies()) {
    for (int i = 0; i < bomberman::GlobalConfig::get().numberOfLobbies(); i++) {
      auto *lobby = resp.add_lobbies();
      lobby->set_id(i);
      lobby->set_connectedplayers(0);
      lobby->set_maxplayers(bomberman::GlobalConfig::get().maxLobbySize());
    }
  }
}

void handleUpdate(const bomberman::UpdateReq &req,
                  bomberman::UpdateResp &resp) {
  if (req.has_lobby()) {
    //
  }
  if (req.has_game()) {
    //
  }
}

bool handleMessage(const bomberman::C2SMessage &req,
                   bomberman::S2CMessage &resp) {
  if (req.has_query()) {
    handleQuery(req.query(), *resp.mutable_query());
  }
  if (req.has_update()) {
    handleUpdate(req.update(), *resp.mutable_update());
  }

  return true;
}

void handleClient(const ClientInfo clientInfo) {
  LOG_INF("[+] Connected from %s:%u", clientInfo.ip.c_str(), clientInfo.port);

  bomberman::common::ConnectionManager connMgr{clientInfo.fd};

  while (auto req = connMgr.receive<bomberman::C2SMessage>()) {
    bomberman::S2CMessage resp;
    if (not handleMessage(req.value(), resp)) {
      break;
    }
    connMgr.send(resp);
  }

  shutdown(clientInfo.fd, SHUT_RDWR);
  close(clientInfo.fd);
  LOG_INF("[-] Disconnected from %s:%u", clientInfo.ip.c_str(),
          clientInfo.port);
}

int main(int argc, char **argv) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  if (not bomberman::GlobalConfig::load(argc, argv)) {
    return 1;
  }

  if (not initNetworking()) {
    LOG_ERR("Networking initialization failed");
    return 1;
  }
  int serverFd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverFd == -1) {
    LOG_ERR("Socket creation failed: %s", strerror(errno));
    return 1;
  }
  int option{1};
  if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) <
      0) {
    LOG_ERR("setsockopt failed: %s", strerror(errno));
    return 1;
  }

  const auto &config{bomberman::GlobalConfig::get()};
  struct sockaddr_in server_sin;
  server_sin.sin_family = AF_INET;
  server_sin.sin_port = htons(config.port());
  server_sin.sin_addr.s_addr = inet_addr(config.bindIp().c_str());

  int status = bind(serverFd, (const struct sockaddr *)&server_sin,
                    sizeof(struct sockaddr_in));
  if (status == -1) {
    LOG_ERR("Bind failed: %s", strerror(errno));
    return 1;
  }
  status = listen(serverFd, config.backlogSize());
  if (status == -1) {
    LOG_ERR("Listen failed: %s", strerror(errno));
    return 1;
  }
  LOG_INF("Listening for incoming connections on %s:%u",
          config.bindIp().c_str(), config.port());
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

  close(serverFd);
  return 0;
}