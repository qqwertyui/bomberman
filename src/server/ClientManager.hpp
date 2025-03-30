#pragma once

namespace bm::common {
struct ConnectionInfo;
}

namespace bm {
class ClientManager {
public:
  static void run();

private:
  static void handleNewConnection(const common::ConnectionInfo &info);
};

} // namespace bm
