#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "ArgumentParser.hpp"
#include "logging/Log.hpp"

namespace bm::common {
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
  ParameterBase() { instances.push_back(this); }

  virtual std::string asString() const = 0;
  virtual std::string getName() const = 0;
  virtual void loadFromString(const std::string &newValue) = 0;

  static const std::vector<ParameterBase *> getInstances() { return instances; }

private:
  inline static std::vector<ParameterBase *> instances;
};

#define PARAMETER(type, name, value)                                           \
  class class_##name : public bm::common::ParameterBase {                      \
  public:                                                                      \
    std::string asString() const override {                                    \
      return #name + std::string("=") + bm::common::toString<type>(m_value);   \
    }                                                                          \
    std::string getName() const override { return #name; }                     \
    const type &operator()() const { return m_value; }                         \
    void loadFromString(const std::string &newValue) override {                \
      m_value = bm::common::fromString<type>(newValue);                        \
    }                                                                          \
                                                                               \
  private:                                                                     \
    type m_value{value};                                                       \
  } name;

template <typename Params> class GlobalConfigBase : public Params {
public:
  GlobalConfigBase(const GlobalConfigBase &) = delete;
  GlobalConfigBase(const GlobalConfigBase &&) = delete;
  GlobalConfigBase &operator=(const GlobalConfigBase &) = delete;
  GlobalConfigBase &operator=(const GlobalConfigBase &&) = delete;

  ~GlobalConfigBase() {
    if (instance) {
      delete instance;
    }
  }

  inline static bool load(int argc, char **argv) {
    instance = new GlobalConfigBase<Params>(argc, argv);
    return instance->good();
  }

  inline static GlobalConfigBase &get() {
    assert(instance != nullptr);
    return *instance;
  }

private:
  GlobalConfigBase(int argc, char **argv) {
    auto cmdLineArgs = common::ArgumentParser::parse(argc, argv);

    for (const auto &[param, value] : cmdLineArgs) {
      if (param == "help") {
        LOG_INF("Possible arguments: %s", asString().c_str());
        return;
      }
      bool found{false};
      for (auto &parameter : ParameterBase::getInstances()) {
        if (parameter->getName() != param) {
          continue;
        }
        parameter->loadFromString(value);
        found = true;
      }
      if (not found) {
        LOG_WRN("Invalid argument: %s", param.c_str());
        return;
      }
    }
    LOG_INF("Loaded configuration: [%s]", asString().c_str());
    initialized = true;
  }

  bool good() const { return initialized; }

  inline std::string asString() {
    std::string result;
    for (const auto &parameter : ParameterBase::getInstances()) {
      result += parameter->asString() + ",";
    }
    result.pop_back();
    return result;
  }

  inline std::string getRegisteredNames() {
    std::string result;
    for (const auto &parameter : ParameterBase::getInstances()) {
      result += parameter->getName() + ",";
    }
    result.pop_back();
    return result;
  }
  bool initialized{false};

  inline static GlobalConfigBase *instance;
};

} // namespace bm::common
