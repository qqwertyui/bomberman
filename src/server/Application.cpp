#include "Application.hpp"

#include "ClientManager.hpp"
#include "GlobalConfig.hpp"
#include "common/Log.hpp"
#include "common/Networking.hpp"

#include <google/protobuf/stubs/common.h>
#include <thread>

namespace bm {
bool Application::initialize(int argc, char **argv) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (not GlobalConfig::load(argc, argv)) {
    return false;
  }
  if (not initNetworking()) {
    LOG_ERR("Networking initialization failed");
    return false;
  }
  return true;
}

int Application::run(int argc, char **argv) {
  if (not initialize(argc, argv)) {
    return 1;
  }
  std::thread{ClientManager::run}.detach();

  while (true) {
  }
  return 0;
}

} // namespace bm
