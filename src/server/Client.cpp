#include "Client.hpp"

#include "GlobalConfig.hpp"
#include "common/Log.hpp"
#include "common/messages/core.pb.h"

namespace bm {
void handleQuery(const QueryReq &req, QueryResp &resp) {
  if (req.has_version()) {
    resp.set_version(GlobalConfig::get().version());
  }
  if (req.has_lobbies()) {
    for (int i = 0; i < GlobalConfig::get().numberOfLobbies(); i++) {
      auto *lobby = resp.add_lobbies();
      lobby->set_id(i);
      lobby->set_connectedplayers(0);
      lobby->set_maxplayers(GlobalConfig::get().maxLobbySize());
    }
  }
}

void handleUpdate(const UpdateReq &req, UpdateResp &resp) {
  if (req.has_lobby()) {
  }
  if (req.has_game()) {
  }
}

bool handleMessage(const C2SMessage &req, S2CMessage &resp) {
  if (req.has_query()) {
    handleQuery(req.query(), *resp.mutable_query());
  }
  if (req.has_update()) {
    handleUpdate(req.update(), *resp.mutable_update());
  }

  return true;
}

void Client::onConnect() {}

std::optional<S2CMessage> Client::onReceive(const C2SMessage &req) {
  S2CMessage resp;
  if (not handleMessage(req, resp)) {
    return std::nullopt;
  }
  return resp;
}

void Client::onDisconnect() {}

} // namespace bm
