#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace bomberman {
class ConfigLoader;
/*
Only string and numeric types are allowed!
*/
template <typename T> inline std::string toString(const T &value) {
  return std::to_string(value);
}
template <> inline std::string toString(const std::string &value) {
  return value;
}

class ParameterBase {
public:
  virtual std::string asString() const = 0;
  virtual std::string getName() const = 0;
};

#define PARAMETER(type, name, value)                                           \
  class class_##name : public ParameterBase {                                  \
  public:                                                                      \
    class_##name() { GlobalConfig::parameters.push_back(this); }               \
    std::string asString() const {                                             \
      return #name + std::string("=") + toString(this->name);                  \
    }                                                                          \
    std::string getName() const { return #name; }                              \
    type &operator()() { return this->name; }                                  \
                                                                               \
  private:                                                                     \
    type name{value};                                                          \
  } name;

/*
Until some universal method is developed, every newly added parameter has to be
added in GlobalConfig class manually by:
- adding PARAMETER entry
- updating handleParameterSpecific in ConfigLoader.cpp
*/
class GlobalConfig {
public:
  GlobalConfig(const GlobalConfig &) = delete;
  GlobalConfig(const GlobalConfig &&) = delete;
  GlobalConfig &operator=(const GlobalConfig &) = delete;
  GlobalConfig &operator=(const GlobalConfig &&) = delete;

  inline static GlobalConfig &get() {
    static GlobalConfig globalConfig;
    return globalConfig;
  }

  inline std::string asString() {
    std::string result;
    for (auto &parameter : parameters) {
      result += parameter->asString() + ",";
    }
    result.pop_back();
    return result;
  }
  inline static std::string getRegisteredNames() {
    std::string result;
    for (auto &parameter : parameters) {
      result += parameter->getName() + ",";
    }
    result.pop_back();
    return result;
  }
  inline static const std::vector<ParameterBase *> &getRegisteredParameters() {
    return parameters;
  }
  inline bool isHelp() const { return isHelpFlag; }

  PARAMETER(uint16_t, maxFps, 60);
  PARAMETER(std::string, assetsDirectory, "assets/");
  PARAMETER(std::string, menuFontPath, "C:/Windows/Fonts/Inkfree.ttf");
  PARAMETER(std::string, serverIp, "127.0.0.1");
  PARAMETER(uint16_t, serverPort, 5551);

private:
  GlobalConfig() = default;

  bool isHelpFlag{false};
  static std::vector<ParameterBase *> parameters;

  friend class ConfigLoader;
};

inline std::vector<ParameterBase *> GlobalConfig::parameters{};

} // namespace bomberman