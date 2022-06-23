#pragma once

#include <map>
#include <string>
#include <vector>

namespace bomberman::common {

class ArgumentParser {
public:
  using ArgumentMap = std::map<std::string, std::string>;
  static ArgumentMap parse(int argc, char **argv);
  static ArgumentMap parse(const std::vector<std::string> &args);
};

} // namespace bomberman::common