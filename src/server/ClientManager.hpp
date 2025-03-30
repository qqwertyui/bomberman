#pragma once

#include "Client.hpp"

namespace bm {

class ClientManager {
public:
  static void run();

private:
  static void handleNewConnection(const Client::Info &info);
};

} // namespace bm
