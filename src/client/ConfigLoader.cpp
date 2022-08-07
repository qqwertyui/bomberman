#include "ConfigLoader.hpp"
#include "GlobalConfig.hpp"
#include "common/ArgumentParser.hpp"
#include "common/Log.hpp"
#include <algorithm>

namespace bomberman {
void ConfigLoader::loadConfig(int argc, char **argv) {
  auto &config = GlobalConfig::get();
  auto cmdLineArgs = common::ArgumentParser::parse(argc, argv);

  for (const auto &[param, value] : cmdLineArgs) {
    if (param == "help") {
      config.isHelpFlag = true;
      break;
    }
    if (not isParameterRegistered(param)) {
      LOG_WRN("Unrecognized argument: %s", param.c_str());
      continue;
    }
    handleParameterSpecific(config, param, value);
  }
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
  } else if (param == "serverIp") {
    config.serverIp() = value;
  } else if (param == "serverPort") {
    config.serverPort() = std::stoi(value);
  } else {
    LOG_WRN("Unhandled argument: %s", param.c_str());
  }
}

} // namespace bomberman