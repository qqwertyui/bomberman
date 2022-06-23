#pragma once

#include <string>

namespace bomberman {

class ConfigLoader {
private:
  struct Config {
    std::string bindIp{"0.0.0.0"};
    unsigned int port{1111};
    unsigned int backlogSize{128};
  };

public:
  ConfigLoader(int argc, char **argv);
  inline const Config &get() const { return cfg; }

private:
  Config cfg;
};

} // namespace bomberman