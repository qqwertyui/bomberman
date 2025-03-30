#include "Client.hpp"

#include "Database.hpp"
#include "GlobalConfig.hpp"
#include "Lobby.hpp"
#include "common/Log.hpp"
#include "common/itf/core.pb.h"

namespace bm {
Client::Client(common::ConnectionInfo connection, unsigned int id)
    : connection(connection), id(id) {}

unsigned int Client::getId() const { return id; }

void handleQuery(const common::itf::QueryReq &req,
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
      buffer->set_connectedplayers(lobby.getMembers().size());
      buffer->set_maxplayers(lobby.getMaxSize());
    }
  }
}

void handleUpdate(const common::itf::UpdateReq &req,
                  common::itf::UpdateResp &resp) {
  if (req.has_lobby()) {
  }
  if (req.has_game()) {
  }
}

bool handleMessage(const common::itf::C2SMessage &req,
                   common::itf::S2CMessage &resp) {
  if (req.has_query()) {
    handleQuery(req.query(), *resp.mutable_query());
  }
  if (req.has_update()) {
    handleUpdate(req.update(), *resp.mutable_update());
  }

  return true;
}

common::ConnectionInfo Client::getConnection() const { return connection; }

void Client::onConnect() {
  LOG_DBG("[+] New connection from %s:%u", connection.ip.c_str(),
          connection.port);
}

std::optional<common::itf::S2CMessage>
Client::onReceive(const common::itf::C2SMessage &req) {
  common::itf::S2CMessage resp;
  if (not handleMessage(req, resp)) {
    return std::nullopt;
  }
  return resp;
}

void Client::onDisconnect() {
  LOG_DBG("[-] Disconnected from %s:%u", connection.ip.c_str(),
          connection.port);
}

} // namespace bm
