#include "ConfigLoader.hpp"
#include "common/ArgumentParser.hpp"

namespace bomberman {
ConfigLoader::ConfigLoader(int argc, char **argv) {
  auto cmdLineArgs = common::ArgumentParser::parse(argc, argv);
  if (cmdLineArgs.count("port")) {
    cfg.port = std::stoi(cmdLineArgs["port"]);
  }
  if (cmdLineArgs.count("bindIp")) {
    cfg.bindIp = cmdLineArgs["bindIp"];
  }
  if (cmdLineArgs.count("backlogSize")) {
    cfg.backlogSize = std::stoi(cmdLineArgs["backlogSize"]);
  }
}
} // namespace bomberman