#include "ClientManager.hpp"

#include "Client.hpp"
#include "Database.hpp"
#include "GlobalConfig.hpp"
#include "common/ConnectionManager.hpp"
#include "common/Log.hpp"
#include "common/Networking.hpp"
#include "common/itf/core.pb.h"
#include <optional>
#include <thread>

namespace bm {
std::optional<int> setupSocket() {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    return std::nullopt;
  }
  int option{1};
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
    return std::nullopt;
  }

  const auto &config{GlobalConfig::get()};
  struct sockaddr_in server_sin;
  server_sin.sin_family = AF_INET;
  server_sin.sin_port = htons(config.port());
  server_sin.sin_addr.s_addr = inet_addr(config.bindIp().c_str());

  if (bind(fd, (const struct sockaddr *)&server_sin,
           sizeof(struct sockaddr_in)) == -1) {
    return std::nullopt;
  }
  if (listen(fd, config.backlogSize()) == -1) {
    return std::nullopt;
  }
  return fd;
}

std::optional<common::ConnectionInfo> acceptConnection(int fd) {
  struct sockaddr_in client_sin;
  socklen_t socklen = sizeof(client_sin);
  int clientFd = accept(fd, (struct sockaddr *)&client_sin, &socklen);
  if (clientFd == -1) {
    return std::nullopt;
  }
  common::ConnectionInfo info{clientFd, inet_ntoa(client_sin.sin_addr),
                              htons(client_sin.sin_port)};
  return info;
}

void ClientManager::run() {
  auto fd = setupSocket();
  if (not fd) {
    LOG_ERR("Socket creation failed: %s", strerror(errno));
    return;
  }
  const auto &config{GlobalConfig::get()};
  LOG_INF("Listening for incoming connections on %s:%u",
          config.bindIp().c_str(), config.port());
  auto &db{Database::get()};
  while (true) {
    if (db.playerLimitReached()) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds{config.playerLimitEvaluationTimerMs()});
      continue;
    }

    auto info{acceptConnection(*fd)};
    if (not info) {
      LOG_ERR("Couldn't accept new connection: %s", strerror(errno));
      continue;
    }
    auto *client = db.addPlayer(*info);
    if (not client) {
      LOG_ERR("Couldn't add new player");
      continue;
    }
    std::thread{handleNewConnection, client}.detach();
  }
  close(*fd);
}

void ClientManager::handleNewConnection(Client *client) {
  common::ConnectionManager connMgr{client->getConnection()};
  client->onConnect();

  while (auto req = connMgr.receive<common::itf::C2SMessage>()) {
    auto resp = client->onReceive(req.value());
    if (not resp) {
      continue;
    }
    connMgr.send(resp.value());
  }

  connMgr.disconnect();
  client->onDisconnect();

  Database::get().removePlayer(client->getId());
}

} // namespace bm
