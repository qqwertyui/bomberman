#pragma once

#include <optional>
#include <string>

namespace bm {
class S2CMessage;
class C2SMessage;

class Client {
public:
  struct Info {
    int fd;
    std::string ip;
    unsigned int port;
  };

  Client(const Info &info);

  void onConnect();
  std::optional<S2CMessage> onReceive(const C2SMessage &req);
  void onDisconnect();

private:
  Info info;
};

} // namespace bm
