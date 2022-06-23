#include "ArgumentParser.hpp"

namespace bomberman::common {
ArgumentParser::ArgumentMap ArgumentParser::parse(int argc, char **argv) {
  std::vector<std::string> args{};
  for (int i = 1; i < argc; i++) {
    args.push_back(argv[i]);
  }
  return parse(args);
}

ArgumentParser::ArgumentMap
ArgumentParser::parse(const std::vector<std::string> &args) {
  ArgumentParser::ArgumentMap result;
  for (const std::string arg : args) {
    std::string first, second;
    size_t firstArgOffset{0};
    if (arg[0] == '-')
      firstArgOffset++;
    if (arg.size() > 1 and arg[1] == '-')
      firstArgOffset++;
    if (firstArgOffset == arg.size()) {
      continue;
    }
    auto secondArgOffset = arg.find('=');
    if (secondArgOffset != std::string::npos) {
      if ((secondArgOffset + 1) < arg.size()) {
        second = arg.substr(secondArgOffset + 1);
      }
    }
    size_t elements = (secondArgOffset == std::string::npos)
                          ? secondArgOffset
                          : secondArgOffset - firstArgOffset;
    first = arg.substr(firstArgOffset, elements);
    result[first] = second;
  }
  return result;
}
} // namespace bomberman::common