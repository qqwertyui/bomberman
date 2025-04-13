#pragma once

#include "common/ConnectionManager.hpp"
#include "common/itf/core.pb.h"
#include <optional>
#include <string>

namespace bm::common {
struct ConnectionInfo;
}

namespace bm {
class Client {
public:
  Client(unsigned int playerId, const common::ConnectionInfo &info);

  void onConnect();
  std::optional<common::itf::C2SMessage> tryReceive();
  std::optional<common::itf::S2CMessage>
  onReceive(const common::itf::C2SMessage &msg);
  void onSend(common::itf::S2CMessage &msg);
  void onDisconnect();

private:
  bool handleMessage(const common::itf::C2SMessage &req,
                     common::itf::S2CMessage &resp);
  void handleQuery(const common::itf::QueryReq &req,
                   common::itf::QueryResp &resp);
  void handleUpdate(const common::itf::UpdateReq &req,
                    common::itf::UpdateResp &resp);
  void handleLobbyUpdate(const common::itf::UpdateLobbyReq &req,
                         common::itf::UpdateLobbyResp &resp);
  void handleGameUpdate(const common::itf::UpdateGameReq &req,
                        common::itf::UpdateGameResp &resp);

  unsigned int playerId;
  common::ConnectionManager connMgr;
  bool shouldExit{false};
  std::optional<unsigned int> lobby;
};

} // namespace bm
