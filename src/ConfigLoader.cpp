#include "ConfigLoader.hpp"
#include "GlobalConfig.hpp"
#include "utils/ArgumentParser.hpp"
#include "utils/Log.hpp"

namespace SimpleSnake {
GlobalConfig ConfigLoader::loadConfig(int argc, char **argv) {
  GlobalConfig config;
  auto args = ArgumentParser::parse(argc, argv);

#ifdef ENABLE_LOGS
  auto registeredParameters = GlobalConfig::getRegisteredParameters();
  std::string allParamsString{};
  for (const auto &param : registeredParameters) {
    allParamsString += param + ",";
  }
  allParamsString.pop_back();
  LOG("Registered configuration parameters: [%s]", allParamsString.c_str());
#endif

  for (const auto &[param, value] : args) {
    if (param == "mapPath") {
      config.mapPath() = value;
    } else if (param == "maxFps") {
      config.maxFps() = std::stoi(value);
    } else {
      LOG("Unrecognized argument: %s", param.c_str());
    }
  }
  return config;
}
} // namespace SimpleSnake