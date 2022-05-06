#include "ConfigLoader.hpp"
#include "utils/Log.hpp"

namespace SimpleSnake {
GlobalConfig ConfigLoader::loadConfig(int argc, char **argv) {
  GlobalConfig config;
  auto args = parseArgs(argc, argv);
  for (const auto &[first, second] : args) {
    if (first == "mapPath") {
      config.mapPath = second;
    } else if (first == "maxFps") {
      config.maxFps = std::stoi(second);
    } else {
      LOG("Unrecognized argument: %s", first.c_str());
    }
  }
  return config;
}

ConfigLoader::ArgumentMap ConfigLoader::parseArgs(int argc, char **argv) {
  ArgumentMap result;
  for (int i = 1; i < argc; i++) {
    std::string arg{argv[i]};
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
        second = arg.substr(secondArgOffset + 1); // --asd=
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
} // namespace SimpleSnake