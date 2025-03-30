#pragma once

#include <optional>
#include <string>

namespace bm {
class S2CMessage;
class C2SMessage;

class Client {
public:
  Client() = default;

  void onConnect();
  std::optional<S2CMessage> onReceive(const C2SMessage &req);
  void onDisconnect();
};

} // namespace bm
