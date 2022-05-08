#pragma once

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

#define PARAMETER(type, name, value)                                           \
  class {                                                                      \
  public:                                                                      \
    std::string asString() const {                                             \
      return #name + std::string("=") + toString(this->name);                  \
    }                                                                          \
    type &operator()() { return this->name; }                                  \
                                                                               \
  private:                                                                     \
    type name{value};                                                          \
  } name;

/*
Until some universal method is developed, every newly added parameter has to be
added in GlobalConfig class manually by adding:
- PARAMETER entry
- updating asString method
- updating getRegisteredParameters method
*/
class GlobalConfig {
public:
  PARAMETER(unsigned int, maxFps, 2);
  PARAMETER(std::string, mapPath, "maps");

  std::string asString() {
    std::string result;
    result += "[";
    result += maxFps.asString() + ", ";
    result += mapPath.asString();
    result += "]";
    return result;
  }
  static std::vector<std::string> getRegisteredParameters() {
    std::vector<std::string> registeredParams{};
    registeredParams.push_back("maxFps");
    registeredParams.push_back("maxPath");
    return registeredParams;
  }
};

} // namespace SimpleSnake