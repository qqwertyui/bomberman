#pragma once

namespace SimpleSnake {
class GlobalConfig;

class ConfigLoader {
public:
  static GlobalConfig loadConfig(int argc, char **argv);
};
} // namespace SimpleSnake