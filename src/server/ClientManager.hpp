#pragma once

namespace bm::common {
namespace itf {
struct C2SMessage;
struct S2CMessage;
} // namespace itf

struct ConnectionInfo;
} // namespace bm::common

namespace bm {
class Client;

class ClientManager {
public:
  static void run();

private:
  static void newClientThreadMain(const common::ConnectionInfo &info);
  static void sendBroadcast(Client *client, common::itf::C2SMessage &req,
                            common::itf::S2CMessage &resp);
};

} // namespace bm
