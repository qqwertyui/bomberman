#pragma once

#include "common/ConnectionInfo.hpp"
#include <optional>
#include <string>

namespace bm::common::itf {
class S2CMessage;
class C2SMessage;
} // namespace bm::common::itf

namespace bm {
class Client {
public:
  Client(common::ConnectionInfo connection, unsigned int id);

  unsigned int getId() const;
  common::ConnectionInfo getConnection() const;

  void onConnect();
  std::optional<common::itf::S2CMessage>
  onReceive(const common::itf::C2SMessage &req);
  void onDisconnect();

private:
  common::ConnectionInfo connection;
  unsigned int id;
};

} // namespace bm
