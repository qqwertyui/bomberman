#include "ConfigLoader.hpp"
#include "GlobalConfig.hpp"
#include "utils/ArgumentParser.hpp"
#include "utils/Log.hpp"
#include <algorithm>

namespace SimpleSnake {
std::unique_ptr<GlobalConfig> ConfigLoader::loadConfig(int argc, char **argv) {
  auto config = std::make_unique<GlobalConfig>();
  auto cmdLineArgs = ArgumentParser::parse(argc, argv);

  for (const auto &[param, value] : cmdLineArgs) {
    if (param == "help") {
      config->isHelpFlag = true;
      break;
    }
    if (not isParameterRegistered(param)) {
      LOG_WRN("Unrecognized argument: %s", param.c_str());
      continue;
    }
    handleParameterSpecific(*config, param, value);
  }
  return config;
}

bool ConfigLoader::isParameterRegistered(const std::string &param) {
  const auto &registeredParameters = GlobalConfig::getRegisteredParameters();
  auto it =
      std::find_if(registeredParameters.begin(), registeredParameters.end(),
                   [&param](ParameterBase *parameter) {
                     return (parameter->getName() == param);
                   });
  return (it != registeredParameters.end());
}

void ConfigLoader::handleParameterSpecific(GlobalConfig &config,
                                           const std::string &param,
                                           const std::string &value) {
  if (param == "assetsDirectory") {
    config.assetsDirectory() = value;
  } else if (param == "maxFps") {
    config.maxFps() = std::stoi(value);
  } else if (param == "menuFontPath") {
    config.menuFontPath() = value;
  } else {
    LOG_WRN("Unhandled argument: %s", param.c_str());
  }
}

} // namespace SimpleSnake