#pragma once

namespace bm::common {
struct ConnectionInfo;
}

namespace bm {
class Client;

class ClientManager {
public:
  static void run();

private:
  static void newClientThreadMain(const common::ConnectionInfo &info);
};

} // namespace bm
