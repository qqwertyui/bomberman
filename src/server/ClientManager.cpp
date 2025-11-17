#include "ClientManager.hpp"

#include "Client.hpp"
#include "Database.hpp"
#include "GlobalConfig.hpp"
#include "Lobby.hpp"
#include "common/Networking.hpp"
#include "common/itf/core.pb.h"
#include "common/logging/Log.hpp"
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
    std::thread{newClientThreadMain, *info}.detach();
  }
  close(*fd);
}

void ClientManager::newClientThreadMain(const common::ConnectionInfo &info) {
  auto [id, client] = Database::get().addPlayer(info);
  if (not client) {
    LOG_ERR("Couldn't add new player");
    return;
  }
  client->onConnect();

  while (auto req = client->tryReceive()) {
    auto resp = client->onReceive(req.value());
    if (not resp) {
      continue;
    }
    client->onSend(resp.value());
    sendBroadcast(client, req.value(), resp.value());
  }

  client->onDisconnect();
  Database::get().removePlayer(id);
}

void ClientManager::sendBroadcast(Client *client, common::itf::C2SMessage &req,
                                  common::itf::S2CMessage &resp) {
  if (not client->lobby) {
    return;
  }
  auto &lobby = Database::get().getLobbyById(*client->lobby);
  for (auto playerIdx : lobby.getMembersIds()) {
    auto *player = Database::get().getPlayerById(playerIdx);
    assert(player != nullptr);
    if (player == client) {
      continue;
    }
    common::itf::S2CMessage msg;
    if (resp.update().game().has_ispositionok() and
        resp.update().game().ispositionok()) {
      auto &position =
          *msg.mutable_ind()->mutable_playerinfo()->mutable_position();
      position.set_x(req.update().game().position().x());
      position.set_y(req.update().game().position().y());
    } else if (req.update().lobby().has_action()) {
      auto &info = *msg.mutable_ind()->mutable_playerinfo();
      info.set_action(req.update().lobby().action());

      if (req.update().lobby().action() == common::itf::LobbyAction::ENTER) {
        auto &position = *info.mutable_position();
        position.set_x(client->getPosition().x);
        position.set_y(client->getPosition().y);
      }
    }
    if (msg.has_ind()) {
      msg.mutable_ind()->mutable_playerinfo()->set_id(client->id());
      player->onSend(msg);
    }
  }
}

} // namespace bm
