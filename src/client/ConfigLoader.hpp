#pragma once

#include <memory>
#include <string>

namespace bomberman {
class GlobalConfig;

class ConfigLoader {
public:
  static std::unique_ptr<GlobalConfig> loadConfig(int argc, char **argv);

private:
  static void handleParameterSpecific(GlobalConfig &config,
                                      const std::string &param,
                                      const std::string &value);
  static bool isParameterRegistered(const std::string &param);
};
} // namespace bomberman