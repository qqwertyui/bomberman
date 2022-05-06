#pragma once

#include <string>

namespace SimpleSnake {

class GlobalConfig {
public:
  std::string mapPath{"maps"};
  unsigned int maxFps{2};

  std::string asString() {
    std::string result;
    result += "mapPath=" + mapPath + ", ";
    result += "maxFps=" + std::to_string(maxFps);
    return result;
  }
};

} // namespace SimpleSnake