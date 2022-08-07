#pragma once

#include <memory>
#include <string>

namespace bomberman {
class GlobalConfig;

class ConfigLoader {
public:
  static void loadConfig(int argc, char **argv);

private:
  static bool isParameterRegistered(const std::string &param);
};
} // namespace bomberman