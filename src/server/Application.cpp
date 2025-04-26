#include "Application.hpp"

#include "ClientManager.hpp"
#include "Database.hpp"
#include "GlobalConfig.hpp"
#include "common/Networking.hpp"
#include "common/logging/Log.hpp"

#include <google/protobuf/stubs/common.h>

namespace bm {
bool Application::initialize(int argc, char **argv) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (not GlobalConfig::load(argc, argv)) {
    return false;
  }
  common::logging::setLevel(
      static_cast<common::logging::Level>(GlobalConfig::get().logLevel()));

  if (not initNetworking()) {
    LOG_ERR("Networking initialization failed");
    return false;
  }
  return true;
}

bool createDatabase() {
  return Database::create(GlobalConfig::get().maxNumberOfLobbies(),
                          GlobalConfig::get().maxNumberOfPlayers());
}

int Application::run(int argc, char **argv) {
  if (not initialize(argc, argv)) {
    return 1;
  }
  if (not createDatabase()) {
    return 2;
  }
  ClientManager::run();
  return 0;
}

} // namespace bm
