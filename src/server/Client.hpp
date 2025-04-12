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
  Client(const common::ConnectionInfo &info);

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
  void handleGameUpdate(const common::itf::UpdateGameReq &req,
                        common::itf::UpdateGameResp &resp);

  common::ConnectionManager connMgr;
  bool shouldExit{false};
};

} // namespace bm
