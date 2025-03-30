#include "ArgumentParser.hpp"
#include <string_view>

namespace bomberman::common {
static constexpr std::string_view prefix{"--"};

bool isValidPrefix(const std::string &arg) {
  return (arg.size() >= prefix.size() and
          arg.substr(0, prefix.size()) == prefix);
}

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
  if (args.size() < 1) {
    return {};
  }

  for (unsigned int i = 0; i < args.size();) {
    const auto &arg1{args[i]};
    if (not isValidPrefix(arg1)) {
      i++;
      continue;
    }
    const auto &name{arg1.substr(prefix.size())};
    std::string value{""};

    int elements{1};
    if (i < (args.size() - 1)) {
      const auto &arg2{args[i + 1]};

      if (not isValidPrefix(arg2)) {
        value = arg2;
        elements = 2;
      }
    }

    result[name] = value;
    i += elements;
  }
  return result;
}
} // namespace bomberman::common