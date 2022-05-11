#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace SimpleSnake {

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
  PARAMETER(unsigned int, maxFps, 60);
  PARAMETER(std::string, assetsDirectory, "assets/");

  std::string asString() {
    std::string result;
    for (auto &parameter : parameters) {
      result += parameter->asString() + ",";
    }
    result.pop_back();
    return result;
  }
  static const std::vector<ParameterBase *> &getRegisteredParameters() {
    return parameters;
  }

private:
  static std::vector<ParameterBase *> parameters;
};

inline std::vector<ParameterBase *> GlobalConfig::parameters{};

} // namespace SimpleSnake