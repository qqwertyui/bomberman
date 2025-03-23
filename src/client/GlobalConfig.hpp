#pragma once

#include <string>
#include <type_traits>
#include <vector>

namespace bomberman {
class ConfigLoader;

// integral types
template <typename T, std::enable_if_t<std::is_integral<T>::value &&
                                           not std::is_same<T, bool>::value,
                                       bool> = true>
inline T fromString(const std::string &value) {
  return std::stoi(value);
}
template <typename T, std::enable_if_t<std::is_integral<T>::value &&
                                           not std::is_same<T, bool>::value,
                                       bool> = true>
inline std::string toString(const T &value) {
  return std::to_string(value);
}

// std::string
template <typename T,
          std::enable_if_t<std::is_same<T, std::string>::value, bool> = true>
inline std::string fromString(const std::string &value) {
  return value;
}
template <typename T,
          std::enable_if_t<std::is_same<T, std::string>::value, bool> = true>
inline std::string toString(const T &value) {
  return value;
}

// boolean types
template <typename T,
          std::enable_if_t<std::is_same<T, bool>::value, bool> = true>
inline bool fromString(const std::string &value) {
  return (value.compare("true") == 0);
}
template <typename T,
          std::enable_if_t<std::is_same<T, bool>::value, bool> = true>
inline std::string toString(const T &value) {
  return value ? "true" : "false";
}

class ParameterBase {
public:
  virtual std::string asString() const = 0;
  virtual std::string getName() const = 0;
  virtual void loadFromString(const std::string &newValue) = 0;
};

#define PARAMETER(type, name, value)                                           \
  class class_##name : public ParameterBase {                                  \
  public:                                                                      \
    class_##name() { GlobalConfig::parameters.push_back(this); }               \
    std::string asString() const override {                                    \
      return #name + std::string("=") + toString<type>(m_value);               \
    }                                                                          \
    std::string getName() const override { return #name; }                     \
    type &operator()() { return m_value; }                                     \
    void loadFromString(const std::string &newValue) override {                \
      m_value = fromString<type>(newValue);                                    \
    }                                                                          \
                                                                               \
  private:                                                                     \
    type m_value{value};                                                       \
  } name;

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
  inline std::string getRegisteredNames() {
    std::string result;
    for (auto &parameter : parameters) {
      result += parameter->getName() + ",";
    }
    result.pop_back();
    return result;
  }
  inline const std::vector<ParameterBase *> &getRegisteredParameters() {
    return parameters;
  }

  PARAMETER(bool, isHelp, false);
  PARAMETER(uint16_t, maxFps, 60);
  PARAMETER(std::string, assetsDir, "assets/");
  PARAMETER(std::string, serverIp, "127.0.0.1");
  PARAMETER(uint16_t, serverPort, 5551);
  PARAMETER(uint16_t, windowWidth, 640);
  PARAMETER(uint16_t, windowHeight, 480);

private:
  GlobalConfig() = default;

  static inline std::vector<ParameterBase *> parameters;
  friend class ConfigLoader;
};

} // namespace bomberman