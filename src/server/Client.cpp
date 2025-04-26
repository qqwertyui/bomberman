#include "Client.hpp"

#include "Database.hpp"
#include "GlobalConfig.hpp"
#include "Lobby.hpp"
#include "common/ConnectionInfo.hpp"
#include "common/itf/core.pb.h"
#include "common/logging/Log.hpp"
#include <cassert>

namespace bm {
Client::Client(unsigned int playerId, const common::ConnectionInfo &info)
    : playerId{playerId}, connMgr{info} {}

void Client::onConnect() {
  auto info{connMgr.getConnectionInfo()};
  assert(info.has_value());

  LOG_DBG("[+] New connection from %s:%u", info->ip.c_str(), info->port);
}

std::optional<common::itf::C2SMessage> Client::tryReceive() {
  if (shouldExit) {
    return std::nullopt;
  }
  return connMgr.receive<common::itf::C2SMessage>();
}

std::optional<common::itf::S2CMessage>
Client::onReceive(const common::itf::C2SMessage &req) {
  common::itf::S2CMessage resp;
  if (not handleMessage(req, resp)) {
    return std::nullopt;
  }
  return resp;
}

bool Client::handleMessage(const common::itf::C2SMessage &req,
                           common::itf::S2CMessage &resp) {
  if (req.has_query()) {
    handleQuery(req.query(), *resp.mutable_query());
  }
  if (req.has_update()) {
    handleUpdate(req.update(), *resp.mutable_update());
  }

  const bool isResponseNeeded{resp.has_query() or resp.has_update()};
  return isResponseNeeded;
}

void Client::handleQuery(const common::itf::QueryReq &req,
                         common::itf::QueryResp &resp) {
  if (req.has_version()) {
    resp.set_version(GlobalConfig::get().version());
  }
  if (req.has_lobbies()) {
    auto &db{Database::get()};
    for (unsigned int i = 0; i < db.getNumberOfLobbies(); i++) {
      const auto &lobby{db.getLobbyById(i)};

      auto *buffer = resp.add_lobbies();
      buffer->set_id(i);
      buffer->set_connectedplayers(lobby.getMembersIds().size());
      buffer->set_maxplayers(lobby.getMaxSize());
    }
  }
}

void Client::handleUpdate(const common::itf::UpdateReq &req,
                          common::itf::UpdateResp &resp) {
  if (req.has_lobby()) {
    handleLobbyUpdate(req.lobby(), *resp.mutable_lobby());
  }
  if (req.has_game()) {
    handleGameUpdate(req.game(), *resp.mutable_game());
  }
}

void Client::handleLobbyUpdate(const common::itf::UpdateLobbyReq &req,
                               common::itf::UpdateLobbyResp &resp) {
  resp.set_status(common::itf::UpdateLobbyResp::OK);

  auto &currentLobbyId{this->lobby};
  const auto targetLobbyId{req.id()};

  const auto maxNumberOfLobbies{Database::get().getNumberOfLobbies()};
  if (targetLobbyId >= maxNumberOfLobbies) {
    LOG_ERR("Player %d sent invalid lobby id: %d (%d available)", playerId,
            targetLobbyId, maxNumberOfLobbies);
    resp.set_status(common::itf::UpdateLobbyResp::LOBBY_NONEXISTENT);
    return;
  }
  auto &targetLobby = Database::get().getLobbyById(targetLobbyId);

  if (req.action() == common::itf::UpdateLobbyReq::ENTER) {
    if (targetLobby.isFull()) {
      resp.set_status(common::itf::UpdateLobbyResp::LOBBY_FULL);
      return;
    }
    if (currentLobbyId.has_value()) {
      auto &currentLobby{Database::get().getLobbyById(currentLobbyId.value())};
      currentLobby.remove(playerId);
    }
    targetLobby.add(playerId);
    currentLobbyId = targetLobbyId;

  } else if (req.action() == common::itf::UpdateLobbyReq::EXIT) {
    if (not currentLobbyId.has_value()) {
      LOG_ERR("Player %d tries to leave lobby %d but doesn't belong to one",
              playerId, targetLobbyId);
      resp.set_status(common::itf::UpdateLobbyResp::INVALID_REQUEST);
      return;
    }
    if (targetLobbyId != currentLobbyId.value()) {
      LOG_ERR("Player %d tries to leave lobby %d but belongs to lobby %d",
              playerId, targetLobbyId, currentLobbyId.value());
      resp.set_status(common::itf::UpdateLobbyResp::INVALID_REQUEST);
      return;
    }
    auto &currentLobby{Database::get().getLobbyById(currentLobbyId.value())};
    currentLobby.remove(playerId);
    currentLobbyId = std::nullopt;
  } else {
    LOG_ERR("Player %d sent invalid lobby request (%d)", playerId,
            static_cast<int>(req.action()));
    resp.set_status(common::itf::UpdateLobbyResp::INVALID_REQUEST);
  }
}

void Client::handleGameUpdate(const common::itf::UpdateGameReq &req,
                              common::itf::UpdateGameResp &resp) {
  if (not lobby.has_value()) {
    LOG_ERR("Player %d sent an game update but doesn't belong to any lobby",
            playerId);
    return;
  }
  if (req.has_position()) {
    [[maybe_unused]] const int x{req.position().x()};
    [[maybe_unused]] const int y{req.position().y()};
    resp.set_ispositionok(true);
  }
}

void Client::onSend(common::itf::S2CMessage &msg) { connMgr.send(msg); }

void Client::onDisconnect() {
  if (lobby.has_value()) {
    auto &currentLobby{Database::get().getLobbyById(lobby.value())};
    currentLobby.remove(playerId);
  }

  auto info{connMgr.getConnectionInfo()};
  assert(info.has_value());

  connMgr.disconnect();
  LOG_DBG("[-] Disconnected from %s:%u", info->ip.c_str(), info->port);
}

} // namespace bm
