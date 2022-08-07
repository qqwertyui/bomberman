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
      config.isHelp() = true;
      LOG_DBG("config.isHelp() = %d", static_cast<int>(config.isHelp()));
      break;
    }
    if (not isParameterRegistered(param)) {
      LOG_WRN("Unrecognized argument: %s", param.c_str());
      continue;
    }
    for (auto &parameter : config.getRegisteredParameters()) {
      if (parameter->getName() == param) {
        parameter->loadFromString(value);
      }
    }
  }
}

bool ConfigLoader::isParameterRegistered(const std::string &param) {
  const auto &registeredParameters =
      GlobalConfig::get().getRegisteredParameters();
  auto it =
      std::find_if(registeredParameters.begin(), registeredParameters.end(),
                   [&param](ParameterBase *parameter) {
                     return (parameter->getName() == param);
                   });
  return (it != registeredParameters.end());
}
} // namespace bomberman