#pragma once

#include "GlobalConfig.hpp"
#include <map>
#include <string>

namespace SimpleSnake {
class ConfigLoader {
public:
  static GlobalConfig loadConfig(int argc, char **argv);

private:
  using ArgumentMap = std::map<std::string, std::string>;
  static ArgumentMap parseArgs(int argc, char **argv);
};
} // namespace SimpleSnake